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

// 212943 - Final Phase Door
Position finalPhaseWalls1[3] =
{
    {-2299.195f, 282.5938f, 408.5445f, 2.383867f},
    {-2250.401f, 234.0122f, 408.5445f, 2.333440f},
    {-2299.63f,  233.3889f, 408.5445f, 0.7598741f}
};

// 212916 - Arena Walls
Position finalPhaseWalls2[3] =
{
    {-2255.168f, 308.7326f, 406.0f,   0.7853968f},
    {-2240.0f,   294.0f,    406.0f,   0.7853968f},
    {-2225.753f, 280.1424f, 406.381f, 0.7853968f},
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
            bool isFlying;
            std::list<uint8> platforms;
            uint8 numPlat;
            uint8 phase;
            uint8 hasTalk;
            uint32 platformToUse;
            uint32 actualPlatform;
            uint8 sonicSpirals;
            uint32 exhaleTarget;
            bool clocksideRings;
            bool isEcho;
            bool isAttEcho;
            bool isFaVEcho;
            bool inhaleDone;
            
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
                if (pInstance)
                {
                    EncounterState bossState = pInstance->GetBossState(DATA_ZORLOK);
                    if (bossState != DONE && bossState != NOT_STARTED)
                        pInstance->SetBossState(DATA_ZORLOK, NOT_STARTED);
                }
                numPlat = 0;
                phase = 0;
                platformToUse = 0;
                hasTalk = 0;
                actualPlatform = 0;
                sonicSpirals = 0;
                clocksideRings = true;
                inhaleDone = false;
                exhaleTarget = 0;

                platforms.clear();
                RemoveWalls();
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

            // --- Specific Functions ---
            void RemoveWalls()
            {
                std::list<GameObject*> arenaList;
                std::list<GameObject*> wallsList;

                GetGameObjectListWithEntryInGrid(arenaList, me, GOB_ARENA_WALLS, 200.0f);
                GetGameObjectListWithEntryInGrid(wallsList, me, GOB_FINAL_PHASE_WALLS, 200.0f);

                for (auto wall : arenaList)
                    me->RemoveGameObject(wall, true);

                for (auto wall : wallsList)
                    me->RemoveGameObject(wall, true);
            }

            void SetFlying()
            {
                isFlying = true;
                me->AttackStop();
                me->DeleteThreatList();
                me->getThreatManager().clearReferences();
                me->SetCanFly(true);
                me->SetReactState(REACT_PASSIVE);
                me->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF);
                me->SetDisableGravity(true);
                me->SendMovementFlagUpdate();
                me->SetByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_HOVER);
                me->AddUnitMovementFlag(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);

                if (isActive)
                {
                    if (phase == PHASE_ZORLOK1 && hasTalk < numPlat)
                    {
                        me->GetMotionMaster()->MovePoint(platformToUse, zorlokReachPoints[platformToUse - 1]);
                        me->MonsterTextEmote("Imperial Vizier Zor'lok is flying to one of hist platforms!", 0, true);
                        hasTalk = numPlat;
                    }
                    else
                        me->GetMotionMaster()->MovePoint(phase, oratiumCenter[0]);
                }
                else
                    me->GetMotionMaster()->MoveTargetedHome();
            }

            void SetLanding(uint8 dest)
            {
                me->SetCanFly(false);
                me->SetDisableGravity(false);
                me->RemoveUnitMovementFlag(MOVEMENTFLAG_FLYING);
                me->RemoveByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_HOVER);
                me->SendMovementFlagUpdate();
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                events.ScheduleEvent(EVENT_INHALE, 15000);

                if (platformToUse != actualPlatform)
                    actualPlatform = platformToUse;

                if (dest)
                {
                    switch (dest)
                    {
                        // Force & Verve platform
                        case PLATFORM_ZORLOK_SW:
                        {
                            events.ScheduleEvent(EVENT_FORCE_AND_VERVE, urand(10000, 20000));
                            break;
                        }
                        // Attenuation Platform
                        case PLATFORM_ZORLOK_NE:
                        {
                            events.ScheduleEvent(EVENT_ATTENUATION, urand(10000, 20000));
                            break;
                        }
                        // Convert Platform
                        case PLATFORM_ZORLOK_SE:
                        {
                            events.ScheduleEvent(EVENT_CONVERT, urand (10000, 20000));
                            break;
                        }
                        // Phase 2 - Center of the room
                        case PHASE_ZORLOK2:
                        {
                            Talk(TALK_EVENT_PHASE_2);
                            events.ScheduleEvent(ChooseAction(), urand(25000, 35000));
                            events.ScheduleEvent(EVENT_PULL_RAID, 2000);
                            break;
                        }
                        default:
                            break;
                    }
                }
            }

            void JustDied(Unit* /*who*/)
            {
                events.Reset();
                summons.DespawnAll();
                me->RemoveAllAreasTrigger();

                if (isEcho)
                    return;

                me->SetCanFly(false);
                me->SetDisableGravity(true);
                RemoveWalls();

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
                // 10 minutes enrage timer
                events.ScheduleEvent(EVENT_BERSERK, 600000);
            }

            void MoveInLineOfSight(Unit* attacker)
            {
                // Do nothing if not in attack phase (ie. flying), or if the unit beside isn't a player
                if (!me->HasReactState(REACT_DEFENSIVE) || attacker->GetTypeId() != TYPEID_PLAYER)
                    return;

                // If is using Song of Empress, stop it
                if (me->HasAura(SPELL_SONG_OF_THE_EMPRESS))
                    me->RemoveAura(SPELL_SONG_OF_THE_EMPRESS);

                // Start attacking player
                me->AddThreat(attacker, 0.0f);
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
                    return platformToUse = numPlat++;

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
                            return platformToUse = platform;
                        }
                    }
                }
            }

            void DamageTaken(Unit* attacker, uint32 &damage)
            {
                // Check if trashes are done
                if (pInstance)
                {
                    EncounterState bossState = pInstance->GetBossState(DATA_ZORLOK);
                    if (bossState == NOT_STARTED || bossState == TO_BE_DECIDED)
                    {
                        Creature* ShieldMaster = GetClosestCreatureWithEntry(me, NPC_SRATHIK_SHIELD_MASTER, 200.0f, true);
                        Creature* Zealok       = GetClosestCreatureWithEntry(me, NPC_ZARTHIK_ZEALOT,        200.0f, true);
                        Creature* Fanatic      = GetClosestCreatureWithEntry(me, NPC_SETTHIK_FANATIC,       200.0f, true);
                        Creature* BoneSmasher  = GetClosestCreatureWithEntry(me, NPC_ENSLAVED_BONESMASHER,  200.0f, true);
                        Creature* Supplicant   = GetClosestCreatureWithEntry(me, NPC_ZARTHIK_SUPPLICANT,    200.0f, true);
                        Creature* Supplicant2  = GetClosestCreatureWithEntry(me, NPC_ZARTHIK_SUPPLICANT_2,  200.0f, true);
                        Creature* Supplicant3  = GetClosestCreatureWithEntry(me, NPC_ZARTHIK_SUPPLICANT_3,  200.0f, true);

                        if (ShieldMaster || Zealok || Fanatic || BoneSmasher || Supplicant || Supplicant2 || Supplicant3)
                        {
                            EnterEvadeMode();
                            pInstance->SetBossState(DATA_ZORLOK, NOT_STARTED);
                            return;
                        }
                    }
                }

                if (isEcho)
                    return;

                if ((isFlying && isActive) || (attacker->GetTypeId() != TYPEID_PLAYER))
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

                // Removing song of the empress
                if (me->GetDistance(attacker) < 5.0f)
                {
                    uint32 spell = me->GetCurrentSpell(CURRENT_CHANNELED_SPELL) ? me->GetCurrentSpell(CURRENT_CHANNELED_SPELL)->m_spellInfo->Id : 0;
                    if (me->HasAura(SPELL_SONG_OF_THE_EMPRESS) || spell == SPELL_SONG_OF_THE_EMPRESS)
                    {
                        me->RemoveAurasDueToSpell(SPELL_SONG_OF_THE_EMPRESS);
                        me->InterruptNonMeleeSpells(true, SPELL_SONG_OF_THE_EMPRESS);
                        me->CastStop(SPELL_SONG_OF_THE_EMPRESS);
                        AttackStart(attacker);
                        me->SetInCombatWith(attacker);
                    }
                }

                if (phase == PHASE_ZORLOK2)
                    return;

                // Phase 1
                if (me->HealthBelowPctDamaged(100 - 20 * numPlat, damage) && phase == PHASE_ZORLOK1)
                {
                    events.Reset();
                    // Switching platforms at 100%, 80% and 60% remaining life
                    if (numPlat < 3)
                    {
                        uint32 platformToUse = ChoosePlatform();
                        me->GetMotionMaster()->MoveTakeoff(platformToUse, zorlokReachPoints[platformToUse - 1]);
                        me->MonsterTextEmote("Imperial Vizier Zor'lok is flying to one of his platforms!", 0, true);
                    }
                    // At 40% remaining phase, switch on phase 2
                    else
                    {
                        phase = PHASE_ZORLOK2;
                        me->GetMotionMaster()->MoveTakeoff(phase, oratiumCenter[0]);
                    }

                    // Set Flying
                    SetFlying();
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
                                me->SetReactState(REACT_DEFENSIVE);
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
                                me->SetReactState(REACT_DEFENSIVE);
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
                        if (inhaleDone)
                            return;

                        me->RemoveAreaTrigger(SPELL_PHEROMONES_CLOUD);
                        pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_PHEROMONES_OF_ZEAL);
                        me->GetMotionMaster()->MoveLand(PHASE_ZORLOK2 + 10,  oratiumCenter[1]);
                        inhaleDone = true;
                        SetLanding(PHASE_ZORLOK2);
                        break;
                    }
                    case ACTION_WIPE:
                    {
                        events.Reset();
                        summons.DespawnAll();

                        me->RemoveAllAreasTrigger();
                        me->RemoveAllAuras();
                        me->SetFullHealth();
                        me->SetReactState(REACT_PASSIVE);

                        isActive = false;
                        platforms.clear();
                        if (!IsHeroic() && !isEcho)
                        {
                            platforms.push_back(PLATFORM_ZORLOK_SW);
                            platforms.push_back(PLATFORM_ZORLOK_NE);
                            platforms.push_back(PLATFORM_ZORLOK_SE);
                        }
                        numPlat = 0;
                        phase = 0;
                        hasTalk = 0;
                        platformToUse = 0;
                        actualPlatform = 0;
                        sonicSpirals = 0;
                        exhaleTarget = 0;
                        clocksideRings = true;
                        inhaleDone = false;
                        RemoveWalls();

                        if (pInstance)
                        {
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_PHEROMONES_CLOUD);
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CONVERT);
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_NOISE_CANCELLING);
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FORCE_AND_VERVE);

                            pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                        }

                        SetFlying();
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
                if (isEcho || id == 0 || type != POINT_MOTION_TYPE)
                    return;

                // General rule :
                // * if id < 4, Zor'lok is going on a platform
                // * if id = 4, Zor'lok is going on the center of the room to absorb pheromones

                // Phase 1 : going onto landing points on platforms
                if (id < 4)
                {
                    if (numPlat == 1)
                        Talk(TALK_EVENT_PHASE_1);
                    me->GetMotionMaster()->MoveLand(id + 10, zorlokPlatforms[id - 1]);
                    SetLanding(id);
                }

                // Switching from phase 1 to phase 2
                if (id == 4)
                {
                    me->CastSpell(me, SPELL_INHALE_PHEROMONES, true);
                    me->MonsterTextEmote("Imperial Vizier Zor'lok inhales Pheromones of Zeal!", 0, true);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_PHEROMONES_CLOUD);
                }
            }

            uint32 GetData(uint32 type)
            {
                if (type == TYPE_EXHALE_TARGET)
                    return exhaleTarget;
                return 0;
            }

            void SetData(uint32 type, uint32 value)
            {
                if (type == TYPE_EXHALE_TARGET)
                    exhaleTarget = value;
            }

            void UpdateAI(const uint32 diff)
            {
                if (!isActive)
                    return;

                // On Wipe
                if (pInstance)
                {
                    if (pInstance->IsWipe())
                    {
                        DoAction(ACTION_WIPE);
                        // We stop here to avoid Zor'lok to cast Song of the empress
                        return;
                    }
                }

                // Remove/Set auras on players
                Map::PlayerList const &PlayerList = me->GetMap()->GetPlayers();
                if (!PlayerList.isEmpty())
                {
                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    {
                        if (Player* playr = i->getSource())
                        {
                            // Remove convert aura for players who have less than 50% remaining health
                            if (playr->HasAura(SPELL_CONVERT) && playr->HealthBelowPct(51))
                                playr->RemoveAurasDueToSpell(SPELL_CONVERT);
                            // Pheromones of zeal - on phase 1 only
                            if (phase == PHASE_ZORLOK1)
                            {
                                // Remove pheromones of zeal aura from players who aren't on the bottom floor
                                if (playr->HasAura(SPELL_PHEROMONES_OF_ZEAL) && playr->GetPositionZ() >= 408.5f)
                                    playr->RemoveAura(SPELL_PHEROMONES_OF_ZEAL);
                                // Set pheromones of zeal aura on players who are on the bottom floor
                                else if (!playr->HasAura(SPELL_PHEROMONES_OF_ZEAL) && playr->GetPositionZ() < 408.5f)
                                    playr->AddAura(SPELL_PHEROMONES_OF_ZEAL, playr);
                            }
                        }
                    }
                }

                UpdateVictim();
                events.Update(diff);

                if (isFlying && platformToUse == actualPlatform && actualPlatform > 0 &&
                    ((me->GetPositionZ() < 410.5f && phase == PHASE_ZORLOK1) ||
                     (me->GetPositionZ() < 407.0f && phase == PHASE_ZORLOK2)))
                {
                    isFlying = false;
                    me->HandleEmoteCommand(EMOTE_ONESHOT_LAND);
                    me->SetReactState(REACT_DEFENSIVE);
                }

                // Song of empress
                Unit* target = me->getVictim();

                if (!isFlying && (!target || me->GetDistance(target) > 5.0f) && !me->HasUnitState(UNIT_STATE_CASTING))
                    me->CastSpell(me, SPELL_SONG_OF_THE_EMPRESS, true);
                
                switch (events.ExecuteEvent())
                {
                    // All-time events
                    case EVENT_INHALE:
                    {
                        // Can't inhale if already casting
                        if (me->HasUnitState(UNIT_STATE_CASTING))
                            events.RescheduleEvent(EVENT_INHALE, 5000);
                        else
                        {
                            // Inhale (Exhale is triggered when Zor'lok has 3-4 stacks of inhale)
                            AuraPtr inhale = me->GetAura(SPELL_INHALE);
                            if (!inhale || inhale->GetStackAmount() < 3 || !urand((inhale->GetStackAmount() < 4 ? 0 : 1), 1))
                            {
                                Talk(TALK_INHALE);
                                me->MonsterTextEmote("Imperial Vizier Zor'lok |cFFFF0000|Hspell:122852|h[Inhale]|h|r a big air breath!", 0, true);
                                me->CastSpell(me, SPELL_INHALE, false);
                            }
                            // Exhale
                            else
                            {
                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 60.0f, true))
                                {
                                    exhaleTarget = target->GetGUIDLow();
                                    Talk(TALK_EXHALE);
                                    DoCast(target, SPELL_EXHALE, true);
                                }
                            }
                            events.ScheduleEvent(EVENT_INHALE, urand(25000, 50000));
                        }
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
                        Talk(TALK_CONVERT);
                        DoCast(SPELL_CONVERT);

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
                        {
                            float x = me->GetPositionX() + frand(-10.0f, 10.0f);
                            float y = me->GetPositionY() + frand(-10.0f, 10.0f);

                            me->CastSpell(x, y, me->GetPositionZ(), SPELL_MISSILE_NOISE_CANC, false);
                        }
                        me->AddUnitState(UNIT_STATE_CASTING);
                        events.ScheduleEvent(EVENT_CAST_FANDV, 2000);
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
                    case EVENT_BERSERK:
                    {
                        me->CastSpell(me, SPELL_BERSERK, false);
                        break;
                    }
                    case EVENT_PULL_RAID:
                    {
                        // Pulling far players
                        std::list<Player*> playerList;
                        GetPlayerListInGrid(playerList, me, 300.0f);
                        for (std::list<Player*>::iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
                            // The point is that if they're on a platform, they'll be blocked by walls, so they have to be pulled
                            if ((*itr)->GetPositionZ() > 408.5f)
                                (*itr)->CastSpell(me, SPELL_SPRING_RABBIT_JUMP, false);

                        // Creating Walls
                        for (uint8 i = 0; i < 3; ++i)
                        {
                            // Arena walls
                            me->SummonGameObject(GOB_ARENA_WALLS, finalPhaseWalls2[i].GetPositionX(), finalPhaseWalls2[i].GetPositionY(), finalPhaseWalls2[i].GetPositionZ(), finalPhaseWalls2[i].GetOrientation(), 0, 0, 0, 0, 7200);

                            // Final phase Doors
                            me->SummonGameObject(GOB_FINAL_PHASE_WALLS, finalPhaseWalls1[i].GetPositionX(), finalPhaseWalls1[i].GetPositionY(), finalPhaseWalls1[i].GetPositionZ(), finalPhaseWalls1[i].GetOrientation(), 0, 0, 0, 0, 7200);
                        }
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
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);

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

                if (phase == PHASE_ZORLOK1)
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

        SpellScript* GetSpellScript() const
        {
            return new spell_inhale_SpellScript();
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
                    caster->SetReactState(REACT_DEFENSIVE);
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
                    caster->AddAura(SPELL_SONIC_RING_AURA, caster);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_sonic_ring_AuraScript::ApplyAura, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectApply += AuraEffectApplyFn(spell_sonic_ring_AuraScript::ApplyAura, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
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
                AfterHit += SpellHitFn(spell_pheromones_of_zeal_SpellScript::Effect);
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

class ExhaleTargetFilter : public std::unary_function<Unit*, bool>
{
    public:
        explicit ExhaleTargetFilter(Unit* caster) : _caster(caster) { }

        bool operator()(WorldObject* object) const
        {
            uint32 exhaleLowId = CAST_AI(boss_zorlok::boss_zorlokAI, _caster->GetAI())->GetData(TYPE_EXHALE_TARGET);
            Player* exhaleTarget = ObjectAccessor::FindPlayer(MAKE_NEW_GUID(exhaleLowId, 0, HIGHGUID_PLAYER));

            if (!exhaleTarget)
                return false;

            // Remove all the other players (stun only Exhale target).
            return (object == exhaleTarget) ? false : true;
        }

    private:
        Unit* _caster;
};

// Exhale: 122761
class spell_zorlok_exhale : public SpellScriptLoader
{
    public:
        spell_zorlok_exhale() : SpellScriptLoader("spell_zorlok_exhale") { }

        class spell_zorlok_exhale_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_zorlok_exhale_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.clear();

                Unit* caster = GetCaster();

                if (!caster)
                    return;

                Player* target = ObjectAccessor::FindPlayer(MAKE_NEW_GUID(TYPEID_PLAYER, 0, caster->GetAI()->GetData(TYPE_EXHALE_TARGET)));

                // No target? Then we pick a random one
                if (!target || !target->isAlive())
                {
                    std::list<Player*> playerList;
                    GetPlayerListInGrid(playerList, caster, 60.0f);

                    if (playerList.empty())
                        return;

                    bool searching = true;
                    std::list<Player*>::iterator itr = playerList.begin();

                    while (searching)
                    {
                        if (urand(0, 1))
                        {
                            target = *itr;
                            searching = false;
                        }
                        ++itr;

                        if (itr == playerList.end())
                            itr = playerList.begin();
                    }
                }

                if (target)
                    targets.push_back(target);
            }

            void RemoveStack()
            {
                if (Unit* caster = GetCaster())
                    caster->RemoveAurasDueToSpell(SPELL_INHALE);
            }

            void HandleScriptEffect(SpellEffIndex effIndex)
            {
                if (Unit* caster = GetCaster())
                    if (Player* target = GetHitPlayer())
                        caster->CastSpell(target, SPELL_EXHALE_DMG, true);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_zorlok_exhale_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_zorlok_exhale_SpellScript::FilterTargets, EFFECT_2, TARGET_UNIT_SRC_AREA_ENEMY);
                AfterCast                += SpellCastFn(spell_zorlok_exhale_SpellScript::RemoveStack);
                OnEffectHitTarget        += SpellEffectFn(spell_zorlok_exhale_SpellScript::HandleScriptEffect, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_zorlok_exhale_SpellScript();
        }
};

// Exhale (damage): 122760
class spell_zorlok_exhale_damage : public SpellScriptLoader
{
    public:
        spell_zorlok_exhale_damage() : SpellScriptLoader("spell_zorlok_exhale_damage") { }

        class spell_zorlok_exhale_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_zorlok_exhale_damage_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                Unit* caster = GetCaster();

                if (targets.empty() || !caster)
                    return;

                Unit* currentTarget = ObjectAccessor::FindPlayer(MAKE_NEW_GUID(TYPEID_PLAYER, 0, caster->GetAI()->GetData(TYPE_EXHALE_TARGET)));
                if (!currentTarget)
                    return;

                // Remove players not between Zorlok and his target.
                std::list<WorldObject*>::iterator itr, next;
                for (itr = targets.begin(); itr != targets.end(); itr = next)
                {
                    next = itr;
                    ++next;

                    // Keeping current target
                    if ((*itr) == currentTarget)
                        continue;

                    if ((*itr)->GetTypeId() != TYPEID_PLAYER || !(*itr)->IsInBetween(caster, currentTarget))
                        targets.remove(*itr);
                }

                // Two or more targets, means there's someone between Zor'lok and his target.
                if (targets.size() > 1)
                {
                    // Select first target between Zor'lok and the Exhale target.
                    WorldObject* nearestTarget = 0;
                    float distance = 1000.0f;

                    for (itr = targets.begin(); itr != targets.end(); ++itr)
                    {
                        if (caster->GetDistance2d(*itr) < distance)
                        {
                            nearestTarget = *itr;
                            distance = caster->GetDistance2d(*itr);
                        }
                    }

                    if (nearestTarget != currentTarget)
                    {
                        // Set Zor'lok's current Exhale target to that nearest player.
                        uint32 targetLowGuid = nearestTarget->GetGUIDLow();
                        caster->GetAI()->SetData(TYPE_EXHALE_TARGET, targetLowGuid);
                    }
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_zorlok_exhale_damage_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_129);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_zorlok_exhale_damage_SpellScript();
        }
};

// 122740 - Convert
class spell_convert : public SpellScriptLoader
{
    public :
        spell_convert() : SpellScriptLoader("spell_convert") { }

        class spell_convertSpellScript : public SpellScript
        {
            PrepareSpellScript(spell_convertSpellScript);

            void SelectTargets(std::list<WorldObject*>& targets)
            {
                targets.clear();

                if (Unit* caster = GetCaster())
                {
                    std::list<Player*> playerList;
                    GetPlayerListInGrid(playerList, caster, 60.0f);

                    // Removing dead or already converted players
                    std::list<Player*>::iterator itr, next;
                    for (itr = playerList.begin(); itr != playerList.end(); itr = next)
                    {
                        next = itr;
                        ++next;

                        if (!(*itr)->isAlive() || (*itr)->HasAura(SPELL_CONVERT))
                            playerList.remove(*itr);
                    }

                    uint8 maxVictims = caster->GetInstanceScript()->instance->Is25ManRaid() ? 5 : 2;
                    // If it remains less players than the number of victims of the spell, the whole raid will be targeted
                    if (playerList.size() <= maxVictims)
                        for (auto player : playerList)
                            targets.push_back(player);
                    // Else, we randomly choose victims until we have enough targets
                    else
                    {
                        std::list<Player*>::iterator itr, next;
                        itr = playerList.begin();
                        while (targets.size() < maxVictims)
                        {
                            next = itr;
                            if (urand(0, 1))
                            {
                                targets.push_back(*itr);
                                playerList.remove(*itr);
                            }
                            itr = ++next;
                            if (itr == playerList.end())
                                itr = playerList.begin();
                        }   
                    }
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_convertSpellScript::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_convertSpellScript();
        }
};

void AddSC_boss_zorlok()
{
    new boss_zorlok();                  // 62980 - Imperial Vizier Zor'lok
    new mob_sonic_ring();               // 62687, 62746, 62744, 62743, 62698, 62699, 62700, 62702, 62703, 62704, 62714, 62715, 62727, 62726, 62719, 62718, 62720, 62721, 62722, 62723, 62724, 62725, 62717, 62716, 62728, 62729, 62696, 62694, 62689, 63340, 63341, 67163, 67164 - Sonic Ring
    new spell_inhale();                 // 122852 - Inhale
    new spell_attenuation();            // 122440 - Attenuation
    new spell_force_verve();            // 122718 - Force and verve
    new spell_sonic_ring();             // 122336 - Sonic Ring
    new spell_pheromones_of_zeal();     // 124018 - Pheromones of Zeal
    new spell_zorlok_exhale();          // 122761 - Exhale
    new spell_zorlok_exhale_damage();   // 122760 - Exhale (damage aura)
    new spell_convert();                // 122740 - Convert
}