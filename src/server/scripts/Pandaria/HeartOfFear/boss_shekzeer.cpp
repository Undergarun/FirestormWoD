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
#include "heart_of_fear.h"

enum eShekzeerSays
{
    SAY_INTRO,          // Entering Shek'zeer's room
    SAY_AGGRO,          // Entering in combat
    SAY_SPELL_CRY,      // Casting Cry of terror
    SAY_SPELL_SERVANT,  // Turning a player into Servant of the Empress
    SAY_RETREAT,        // Empress returns in chrysalid
    SAY_ADVANCE,        // Empress goes out from chrysalid
    SAY_POWER,          // Empress near 30%
    SAY_WIPE,           // Raid has been wiped
    SAY_SLAY,           // Killing a player
    SAY_DEATH,          // Empress dies
    SAY_START,          // ??? - Said just before SAY_CONTROL (next enum)
};

enum eShaSays
{
    SAY_DEAD_QUEEN,     // Empress is dead
    SAY_LEAVING,        // Leaving boss' room after Empress is defeated
    SAY_LAST_PHASE,     // Entering in phase 3
    SAY_PREPARE_ADVANCE,// Preparing return in phase 1
    SAY_CONTROL,        // ???
};

enum eShekzeedAdds
{
    NPC_SETTHIK_WINDBLADE           = 63589,
    NPC_KORTHIK_REAVER              = 63591,
    NPC_DISSONANCE_FIELD            = 62847,
    NPC_SHA_OF_FEAR                 = 63942,
    NPC_STICKY_RESIN                = 63730,
    NPC_AMBER_TRAP                  = 64351,
};

enum eShekzeerSpells
{
    // --- Shek'zeer ---
    // Phase 1
    SPELL_RETREAT                   = 125098,
    SPELL_ADVANCE                   = 125304,
    SPELL_END_ADVANCE               = 125306,
    SPELL_SUMMON_DISSONANCE         = 124856,
    SPELL_SHA_AURA_DISSONANCE       = 125296,
    SPELL_SHA_DISSONANCE            = 123819,
    SPELL_CRY_OF_TERROR             = 123788,
    SPELL_EYES_OF_THE_EMPRESS       = 123707,
    SPELL_SERVANT_OF_THE_EMPRESS    = 123723,
    SPELL_DREAD_SCREECH             = 123735,
    // Phase 2
    SPELL_SHA_ENERGY                = 125464,
    SPELL_CONSUMING_TERROR          = 124849,
    SPELL_VISIONS_OF_DEMISE         = 124862,
    SPELL_CALAMITY                  = 124845,
    SPELL_AMASSING_DARKNESS         = 124842,
    SPELL_AMASSING_DMG              = 124844,

    // --- Set'thik Windblade ---
    SPELL_FIXATE                    = 125390,
    SPELL_ADD_DISPATCH              = 124077,
    SPELL_ADD_SONIC_BLADE           = 125886,
    SPELL_STICKY_RESIN              = 124092,   // Missile which trigger 124095 which summons NPC_STICKY_RESIN (63730)

    // --- Sticky Resin ---
    SPELL_STICKY_RESIN_VISUAL       = 124310,   // Visual of little sticky resin
    SPELL_STICKY_DAMAGES            = 124097,
    SPELL_MERGED_RESIN              = 125719,   // Visual when 2 little sticky resins merge into 1 big
    SPELL_AMBER_TRAP                = 125826,   // Amber trap visual
    SPELL_TRAPPED                   = 125823,   // Traps the mob into amber trap and freezes the victim
    SPELL_RESIN_SPURT               = 124748,   // Little resin spurt when 2 sticky resins merge

    // --- Amber Trap ---
    SPELL_TRAP_DAMAGES              = 125822,
    SPELL_FREEZE_ANIM               = 16245,

    // --- Kor'thik Reaver
    SPELL_POISON_BOMB               = 124777,
    SPELL_TOXIC_SLIME               = 124807,
    SPELL_POISON_DRENCHED_ARMOR     = 124821,

    // --- Dissonance Field ---
    SPELL_DISSONANCE_FIELDS         = 123184,
    SPELL_DISSONANCE_SPELLS         = 123627,
    SPELL_CORRUPTED_DISSONANCE      = 126122,
    SPELL_DISSONANCE_FIELD_AURA     = 123255,
    SPELL_SONIC_DISCHARGE           = 123504,
    SPELL_RESONANCE                 = 123618,

    // --- Sha of fear ---
    SPELL_SHA_OF_FEAR               = 124905,
    SPELL_ULTIMATE_CORRUPTION       = 125451,
};

enum eShekzeerEvents
{
    // Shek'zeer
    EVENT_CHANGE_PHASE = 1, // Switching to phase 1 when in phase 2 after 150 secs
    EVENT_POWER_DECREASE,   // In phase 1, power decrease by 1 every second
    EVENT_DISSONANCE_FIELDS,
    EVENT_CRY_OF_TERROR,
    EVENT_EYES_OF_THE_EMPRESS,
    EVENT_DREAD_SCREECH,
    EVENT_SHA_ENERGY,
    EVENT_CONSUMING_TERROR,
    EVENT_VISIONS_OF_DEMISE,
    EVENT_CALAMITY,
    EVENT_AMASSING_DARKNESS,
    EVENT_LOAD_PHASE,
    EVENT_CLOSE_CHAMBER,
    EVENT_BERSERK,          // 9 min in normal mode, 15 in heroic

    // Set'thik Windblade
    EVENT_ADD_DISPATCH,
    EVENT_ADD_SONIC_BLADE,
    EVENT_STICKY_RESIN,

    // Sticky Resin
    EVENT_REARM_TRAP,
    EVENT_CHECK_INTERRACT,

    // Kor'thik Reaver
    EVENT_POISON_BOMB,
    EVENT_TOXIC_SLIME,
    EVENT_POISON_DRENCHED_ARMOR,

    // Dissonance Field
    EVENT_CHECK_CAST,
    
    // Sha of Fear
    EVENT_LEAVING,
};

enum eShekzeerActions
{
    ACTION_LOAD_PHASE,
    ACTION_RETREAT,
    ACTION_COMBAT,
    ACTION_ALL_ADD_DIED,
    ACTION_RESIN_GROW,
    ACTION_TRAP_END,
    ACTION_FREE_ALLY,
    ACTION_FIGHTBACK,
    ACTION_LAST_PHASE,
    ACTION_WITHDRAW,
    ACTION_LEAVING,
};

enum eShekzeerTypes
{
    TYPE_RETREAT,
    TYPE_END_FIGHT,
};

#define TIME_PHASE_DELAY 150000

Position chamberPos = {-2478.769f, 1068.421f, 573.1021f, 1.570796f};
Position attackPos  = {-2478.769f, 1008.200f, 572.8200f, 4.712389f};
Position leavingPos = {-2478.520f, 1013.510f, 569.6300f, 4.712389f};
Position highPos    = {-2478.520f, 1013.510f, 770.0000f, 4.712389f};

Position addSpawnLeft[4] =
{
    {-2360.0f, 1014.0f, 575.90f, 3.141593f},
    {-2352.0f, 1014.0f, 575.90f, 3.141593f},
    {-2352.0f, 1006.0f, 575.90f, 3.141593f},
    {-2352.0f, 1022.0f, 575.90f, 3.141593f},
};

Position addSpawnRight[4] =
{
    {-2600.0f, 1014.0f, 575.90f, 0.0f},
    {-2608.0f, 1014.0f, 575.90f, 0.0f},
    {-2608.0f, 1006.0f, 575.90f, 0.0f},
    {-2608.0f, 1022.0f, 575.90f, 0.0f},
};

// Zorlok - 62837
class boss_shekzeer : public CreatureScript
{
    public:
        boss_shekzeer() : CreatureScript("boss_shekzeer") { }

        struct boss_shekzeerAI : public BossAI
        {
            boss_shekzeerAI(Creature* creature) : BossAI(creature, DATA_SHEKZEER)
            {
                pInstance = creature->GetInstanceScript();
                loaded = false;
                me->SetDisplayId(DISPLAYID_INVISIBLE);

                if (pInstance)
                    if (CheckTrash() && pInstance->CheckRequiredBosses(DATA_SHEKZEER))
                        DoAction(ACTION_COMBAT);

                if (isWipe)
                    isWipe = false;
            }

            InstanceScript* pInstance;
            EventMap events;
            uint8 phase;
            bool fightInProgress;
            bool isWipe;
            bool loaded;
            bool isInChamber;
            bool introDone;
            bool aggroTalkDone;
            bool hasRequiredPower;

            void Reset()
            {
                events.Reset();
                summons.DespawnAll();
                phase = 0;
                fightInProgress  = false;
                isWipe           = false;
                introDone        = false;
                aggroTalkDone    = false;
                hasRequiredPower = false;
                _Reset();
                if (pInstance)
                    if (pInstance->GetBossState(DATA_SHEKZEER) == FAIL)
                        pInstance->SetBossState(DATA_SHEKZEER, NOT_STARTED);

                me->SetReactState(REACT_DEFENSIVE);
                me->SetDisplayId(DISPLAYID_INVISIBLE);
                isInChamber = true;
            }

            bool CheckTrash()
            {
                if (!GetClosestCreatureWithEntry(me, NPC_KORTHIK_WARSINGER, 200.0f))
                    if (!GetClosestCreatureWithEntry(me, NPC_ZARTHIK_AUGURER, 200.0f))
                        if (!GetClosestCreatureWithEntry(me, NPC_SETTHIK_WINDBLADE_TRASH, 200.0f))
                            return true;
                return false;
            }

            void MoveInLineOfSight(Unit* who)
            {
                if (introDone)
                    return;

                if (who->GetTypeId() == TYPEID_PLAYER)
                {
                    if (who->GetDistance(me) < 240.7f)
                    {
                        Talk(SAY_INTRO);
                        introDone = true;
                    }
                }
            }

            void EnterCombat(Unit* attacker)
            {
                if (fightInProgress || !pInstance)
                    return;

                if (!CheckTrash() || !pInstance->CheckRequiredBosses(DATA_SHEKZEER))
                {
                    EnterEvadeMode();
                    return;
                }

                if (!aggroTalkDone)
                {
                    Talk(SAY_AGGRO);
                    aggroTalkDone = true;
                }

                fightInProgress = true;
                pInstance->SetBossState(DATA_SHEKZEER, IN_PROGRESS);
                pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                DoZoneInCombat();
                phase = 1;
                me->SetInCombatWith(attacker);
                AttackStart(attacker);

                events.ScheduleEvent(EVENT_DREAD_SCREECH, 6000);
                events.ScheduleEvent(EVENT_EYES_OF_THE_EMPRESS, urand(10000, 15000));
                events.ScheduleEvent(EVENT_DISSONANCE_FIELDS, 30000);
                events.ScheduleEvent(EVENT_CRY_OF_TERROR, 25000);
                events.ScheduleEvent(EVENT_BERSERK, IsHeroic() ? 900000 : 540000);
                events.ScheduleEvent(EVENT_CLOSE_CHAMBER, 5000);
                events.ScheduleEvent(EVENT_CHANGE_PHASE, TIME_PHASE_DELAY);
            }

            void JustDied(Unit* /*killer*/)
            {
                Talk(SAY_DEATH);

                events.Reset();
                summons.DespawnAll();

                // Despawn remaining amber traps and sticky resin
                uint32 amberEntries[2] = {NPC_STICKY_RESIN, NPC_AMBER_TRAP};
                for (uint32 mob : amberEntries)
                {
                    std::list<Creature*> addList;
                    GetCreatureListWithEntryInGrid(addList, me, mob, 200.0f);

                    for (Creature* addMob : addList)
                        addMob->DespawnOrUnsummon();
                }

                // Sha ending
                if (Creature* sha = GetClosestCreatureWithEntry(me, NPC_SHA_OF_FEAR, 200.0f))
                    sha->AI()->DoAction(ACTION_WITHDRAW);

                phase = 0;
                if (pInstance)
                {
                    pInstance->SetBossState(DATA_SHEKZEER, DONE);
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_EYES_OF_THE_EMPRESS);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_VISIONS_OF_DEMISE);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_AMASSING_DARKNESS);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CONVERT_SERVANT);
                }

                fightInProgress = false;

                _JustDied();
            }

            void KilledUnit(Unit* victim)
            {
                if (victim->GetTypeId() == TYPEID_PLAYER)
                    Talk(SAY_SLAY);
            }

            void JustReachedHome()
            {
                if (pInstance)
                {
                    if (pInstance->IsWipe())
                    {
                        me->SetReactState(REACT_DEFENSIVE);
                        // Sha becomes invisible
                        if (Creature* sha = GetClosestCreatureWithEntry(me, NPC_SHA_OF_FEAR, 200.0f))
                            sha->SetDisplayId(DISPLAYID_INVISIBLE);
                    }
                }

                isInChamber = true;
                me->SetFacingTo(M_PI * 3 / 2);
                me->SetDisplayId(DISPLAYID_INVISIBLE);

                if (GameObject* chrysalid = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_EMPRESS_CHAMBER)))
                    if (chrysalid->GetGoState() == GO_STATE_ACTIVE)
                        chrysalid->SetGoState(GO_STATE_READY);
            }

            void EnterEvadeMode()
            {
                events.Reset();
                summons.DespawnAll();
                // Reset variables
                phase = 0;
                fightInProgress  = false;
                isWipe           = false;
                introDone        = false;
                aggroTalkDone    = false;
                hasRequiredPower = false;
                // Reset behaviour
                me->RemoveAllAuras();
                me->SetDisplayId(DISPLAYID_INVISIBLE);
                me->SetFullHealth();
                me->SetReactState(REACT_PASSIVE);
                me->GetMotionMaster()->MoveTargetedHome();
                if (pInstance)
                {
                    if (isWipe)
                        pInstance->SetBossState(DATA_SHEKZEER, FAIL);
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    // Remove auras on players
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CRY_OF_TERROR);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_AMASSING_DARKNESS);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_EYES_OF_THE_EMPRESS);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_VISIONS_OF_DEMISE);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CONVERT_SERVANT);
                }
                _EnterEvadeMode();
            }

            void DamageTaken(Unit* attacker, uint32 &damage)
            {
                if (phase == 3)
                    return;

                if (!phase)
                    EnterCombat(attacker);

                // Near entering phase 3
                if (me->HealthBelowPctDamaged(31, damage))
                {
                    if (!hasRequiredPower)
                    {
                        Talk(SAY_POWER);
                        hasRequiredPower = true;
                    }
                }

                // Entering in phase 3
                if (me->HealthBelowPctDamaged(30, damage))
                {
                    // Cancelling phase 1 events
                    events.CancelEvent(EVENT_DREAD_SCREECH);
                    events.CancelEvent(EVENT_DISSONANCE_FIELDS);
                    events.CancelEvent(EVENT_CRY_OF_TERROR);
                    events.CancelEvent(EVENT_CHANGE_PHASE);

                    // Scheduling phase 3 events. NB : Eyes of empress remains in phase 3.
                    events.ScheduleEvent(EVENT_VISIONS_OF_DEMISE, 6000);
                    events.ScheduleEvent(EVENT_CALAMITY, 10000);
                    events.ScheduleEvent(EVENT_SHA_ENERGY, 15000);
                    events.ScheduleEvent(EVENT_AMASSING_DARKNESS, 22000);

                    // Breaking the chrysalid
                    if (GameObject* chrysalid = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_EMPRESS_CHAMBER)))
                        chrysalid->SetGoState((GO_STATE_ACTIVE_ALTERNATIVE));

                    if (Creature* sha = GetClosestCreatureWithEntry(me, NPC_SHA_OF_FEAR, 200.0f))
                        sha->AI()->DoAction(ACTION_LAST_PHASE);

                    phase = 3;
                }
            }

            void DoAction(const int32 action)
            {
                if (!me->isAlive())
                    return;

                switch (action)
                {
                    // Entering in phase 2
                    case ACTION_RETREAT:
                    {
                        events.CancelEvent(EVENT_DREAD_SCREECH);
                        events.CancelEvent(EVENT_DISSONANCE_FIELDS);
                        events.CancelEvent(EVENT_EYES_OF_THE_EMPRESS);
                        events.CancelEvent(EVENT_CRY_OF_TERROR);

                        DoCast(SPELL_RETREAT);
                        me->SetReactState(REACT_PASSIVE);
                        me->SetSpeed(MOVE_WALK, 10.0f);
                        me->GetMotionMaster()->Clear();
                        me->GetMotionMaster()->MovePoint(1, chamberPos);

                        Talk(SAY_RETREAT);
                        break;
                    }
                    case ACTION_ALL_ADD_DIED:
                    {
                        // We cancel the scheduled event as we enter in phase 1 immediately
                        events.CancelEvent(EVENT_CHANGE_PHASE);
                        // NO BREAK!!!
                    }
                    // Entering in phase 1
                    case ACTION_COMBAT:
                    case ACTION_SHEKZEER_COMBAT:
                    {
                        // Loaded used to prevent several calls to this action
                        if (!loaded)
                            loaded = true;

                        if (!pInstance)
                            return;

                        if (!pInstance->CheckRequiredBosses(DATA_SHEKZEER))
                        {
                            EnterEvadeMode();
                            return;
                        }

                        if (Creature* sha = GetClosestCreatureWithEntry(me, NPC_SHA_OF_FEAR, 200.0f))
                            sha->AI()->DoAction(ACTION_COMBAT);

                        DoCast(SPELL_ADVANCE);
                        // Opening chamber
                        if (pInstance)
                            if (GameObject* chamber = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_EMPRESS_CHAMBER)))
                                if (chamber->GetGoState() == GO_STATE_READY)
                                    chamber->SetGoState(GO_STATE_ACTIVE);

                        me->SetDisplayId(me->GetNativeDisplayId());
                        me->SetSpeed(MOVE_WALK, 10.0f);
                        me->GetMotionMaster()->MovePoint(2, attackPos);
                        break;
                    }
                    default:
                        break;
                }
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type != POINT_MOTION_TYPE || !id)
                    return;

                switch (id)
                {
                    // Retreats in chamber
                    case 1:
                    {
                        if (phase == 2)
                            return;

                        me->SetSpeed(MOVE_WALK, 1.0f);
                        isInChamber = true;

                        // Not visible anymore
                        me->SetDisplayId(DISPLAYID_INVISIBLE);
                        if (Creature* sha = GetClosestCreatureWithEntry(me, NPC_SHA_OF_FEAR, 200.0f))
                        {
                            sha->RemoveAllAuras();
                            if(sha->GetDisplayId() == sha->GetNativeDisplayId())
                                sha->SetDisplayId(DISPLAYID_INVISIBLE);
                        }

                        // Closing chamber
                        if (pInstance)
                            if (GameObject* chamber = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_EMPRESS_CHAMBER)))
                                if (chamber->GetGoState() == GO_STATE_ACTIVE)
                                    chamber->SetGoState(GO_STATE_READY);

                        // Spawn trash for phase 2
                        for (uint8 i = 0; i < 4; ++i)
                        {
                            // Spawning 3 Set'thik Windblade on each side
                            if (i)
                            {
                                me->SummonCreature(NPC_SETTHIK_WINDBLADE, addSpawnLeft[i]);
                                me->SummonCreature(NPC_SETTHIK_WINDBLADE, addSpawnRight[i]);
                            }
                            // Spawning 1 Kor'thik Reaver on each side (i == 0, here)
                            else
                            {
                                me->SummonCreature(NPC_KORTHIK_REAVER, addSpawnLeft[i]);
                                me->SummonCreature(NPC_KORTHIK_REAVER, addSpawnRight[i]);
                            }
                        }

                        // Phase 2 activated
                        events.ScheduleEvent(EVENT_CHANGE_PHASE, TIME_PHASE_DELAY);
                        phase = 2;
                        break;
                    }
                    // Combat position
                    case 2:
                    {
                        if (phase == 1)
                            return;

                        isInChamber = false;
                        me->SetSpeed(MOVE_WALK, 1.0f);
                        DoCast(SPELL_END_ADVANCE);

                        // If trash just died, defensive react, else, boss aggro
                        me->SetReactState(phase ? REACT_AGGRESSIVE : REACT_DEFENSIVE);


                        // if phase = 0, we'll enter in phase 1 in EnterCombat, else, we immediately return from phase 2 to phase 1
                        if (phase)
                        {
                            phase = 1;

                            Talk(SAY_ADVANCE);

                            // Scheduling phase 1 events
                            events.ScheduleEvent(EVENT_DREAD_SCREECH, 6000);
                            events.ScheduleEvent(EVENT_EYES_OF_THE_EMPRESS, urand(10000, 15000));
                            events.ScheduleEvent(EVENT_DISSONANCE_FIELDS, 30000);
                            events.ScheduleEvent(EVENT_CRY_OF_TERROR, 25000);
                            events.ScheduleEvent(EVENT_CLOSE_CHAMBER, 5000);
                            events.ScheduleEvent(EVENT_CHANGE_PHASE, TIME_PHASE_DELAY);
                        }
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (pInstance)
                {
                    if (pInstance->IsWipe() && !isWipe)
                    {
                        EnterEvadeMode();
                        Talk(SAY_WIPE);
                        isWipe = true;
                        return;
                    }
                    else if (isWipe)
                        isWipe = false;
                }

                if (!loaded)
                {
                    if (CheckTrash())
                        DoAction(ACTION_COMBAT);
                    else
                        return;
                }

                UpdateVictim();

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        // --- Generic events ---
                        case EVENT_CHANGE_PHASE:
                        {
                            // Opening chrysalid
                            if (phase == 1)
                                if (GameObject* chrysalid = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_EMPRESS_CHAMBER)))
                                    if (chrysalid->GetGoState() == GO_STATE_READY)
                                        chrysalid->SetGoState(GO_STATE_ACTIVE);

                            if (phase == 2)
                                if (Creature* sha = GetClosestCreatureWithEntry(me, NPC_SHA_OF_FEAR, 200.0f))
                                    sha->AI()->Talk(SAY_PREPARE_ADVANCE);

                            events.ScheduleEvent(EVENT_LOAD_PHASE, 5000);
                            break;
                        }
                        case EVENT_LOAD_PHASE:
                        {
                            // triggers ACTION_RETREAT (1) if phase = 1 or ACTION_COMBAT (2) if phase = 2
                            DoAction(phase);
                            break;
                        }
                        case EVENT_CLOSE_CHAMBER:
                        {
                            if (GameObject* chamber = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_EMPRESS_CHAMBER)))
                                chamber->SetGoState(GO_STATE_READY);
                            break;
                        }
                        case EVENT_EYES_OF_THE_EMPRESS:
                        {
                            if (Unit* target = me->getVictim())
                                me->CastSpell(target, SPELL_EYES_OF_THE_EMPRESS, true);
                            events.ScheduleEvent(EVENT_EYES_OF_THE_EMPRESS, urand(10000, 15000));
                            break;
                        }
                        // --- Phase 1 events ---
                        case EVENT_DREAD_SCREECH:
                        {
                            DoCast(SPELL_DREAD_SCREECH);
                            events.ScheduleEvent(EVENT_DREAD_SCREECH, 6000);
                            break;
                        }
                        case EVENT_DISSONANCE_FIELDS:
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            {
                                me->CastSpell(target, SPELL_SUMMON_DISSONANCE, true);
                                me->CastSpell(target, SPELL_SUMMON_DISSONANCE, true);
                                DoCast(me, SPELL_SHA_AURA_DISSONANCE);
                                DoCast(me, SPELL_SHA_DISSONANCE);
                            }
                            events.ScheduleEvent(EVENT_DISSONANCE_FIELDS, 30000);
                            break;
                        }
                        case EVENT_CRY_OF_TERROR:
                        {
                            std::list<Player*> playerList;
                            GetPlayerListInGrid(playerList, me, 200.0f);

                            if (!playerList.empty())
                            {
                                // Picking a random target
                                std::list<Player*>::iterator itr, next;
                                itr = playerList.begin();
                                Player* target = 0;
                                bool canFind = true;

                                while (!target && canFind)
                                {
                                    next = itr;
                                    ++next;

                                    // Target must not be a Tank, a melee DPS or a hunter
                                    if ((*itr)->GetRoleForGroup() == ROLES_TANK ||
                                        (*itr)->GetRoleForGroup() == ROLES_DPS  ||
                                        (*itr)->getClass() == CLASS_HUNTER)
                                    {
                                        playerList.remove(*itr);
                                        if (playerList.empty())
                                            canFind = false;
                                    }
                                    else if (urand(0, 1))
                                        target = *itr;

                                    itr = next;
                                    if (itr == playerList.end())
                                        itr = playerList.begin();
                                }

                                if (!target)
                                    break;

                                DoCast(target, SPELL_CRY_OF_TERROR);
                                Talk(SAY_SPELL_CRY);
                            }
                            break;
                        }
                        // --- Phase 2 events ---
                        case EVENT_SHA_ENERGY:
                        {
                            DoCast(SPELL_SHA_ENERGY);
                            events.ScheduleEvent(EVENT_SHA_ENERGY, urand(5000, 15000));
                            break;
                        }
                        case EVENT_CONSUMING_TERROR:
                        {
                            DoCast(SPELL_CONSUMING_TERROR);
                            events.ScheduleEvent(EVENT_CONSUMING_TERROR, urand(30000, 40000));
                            break;
                        }
                        case EVENT_VISIONS_OF_DEMISE:
                        {
                            uint8 maxTarget = Is25ManRaid() ? 5 : 2;
                            std::list<Player*> playerList;

                            GetPlayerListInGrid(playerList, me, 200.0f);

                            if (playerList.size() > maxTarget)
                            {
                                std::list<Player*>::iterator itr, next;
                                itr = playerList.begin();

                                // Reducing playerlist until it matches the number of targets
                                while (playerList.size() > maxTarget)
                                {
                                    next = itr;
                                    ++next;

                                    if (urand(0, 1))
                                        playerList.remove(*itr);

                                    itr = next;
                                }
                            }

                            for (Player* player : playerList)
                                me->CastSpell(player, SPELL_VISIONS_OF_DEMISE, true);

                            events.ScheduleEvent(EVENT_VISIONS_OF_DEMISE, urand(15000, 20000));
                            break;
                        }
                        case EVENT_CALAMITY:
                        {
                            DoCast(SPELL_CALAMITY);
                            events.ScheduleEvent(EVENT_CALAMITY, urand(6000, 10000));
                            break;
                        }
                        case EVENT_AMASSING_DARKNESS:
                        {
                            DoCast(SPELL_AMASSING_DARKNESS);
                            break;
                        }
                        case EVENT_BERSERK:
                        {
                            DoCast(me, SPELL_BERSERK);
                            break;
                        }
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_shekzeerAI(creature);
        }
};

// Applying buff to adds which are less than 8.0f distance from each other
void ShekzeerBuffAdds(Creature* me)
{
    uint32 addEntries[2] = {NPC_SETTHIK_WINDBLADE, NPC_KORTHIK_REAVER};

    bool buff = false;
    uint8 stacks = 0;
    for (uint8 i = 0; i < 2; ++i)
    {
        std::list<Creature*> addList;
        GetCreatureListWithEntryInGrid(addList, me, addEntries[i], 8.0f);

        // Retaining only alive mobs who aren't me
        for (Creature* mob : addList)
        {
            if (mob->isAlive() && mob != me)
            {
                if (!buff)
                    buff = true;
                ++stacks;
            }
        }
    }

    // If buff should be applied, we have to check that we have the right number of stacks
    if (buff)
    {
        AuraPtr aura = me->AddAura(SPELL_BAND_OF_VALOR, me);

        if (aura->GetStackAmount() != stacks)
            aura->SetStackAmount(stacks);
    }

    // Remove aura if applied and there's no add around
    if (!buff && me->HasAura(SPELL_BAND_OF_VALOR))
        me->RemoveAura(SPELL_BAND_OF_VALOR);
}

// 63589 - Set'thik Windblade
class mob_add_setthik_windblade : public CreatureScript
{
    public:
        mob_add_setthik_windblade() : CreatureScript("mob_add_setthik_windblade") { }

        struct mob_add_setthik_windbladeAI : public ScriptedAI
        {
            mob_add_setthik_windbladeAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* pInstance;
            uint64 fixTarget;

            void Reset()
            {
                events.Reset();

                me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, EQUIP_TRASH_6);
                me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 1, EQUIP_TRASH_6);

                DoZoneInCombat();

                // Fixate
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                {
                    me->CastSpell(target, SPELL_FIXATE, true);
                    fixTarget = target->GetGUID();
                    me->SetInCombatWith(target);
                    AttackStart(target);
                }

                events.ScheduleEvent(EVENT_DISPATCH, 7000);
                events.ScheduleEvent(EVENT_SONIC_BLADE, 22000);
                events.ScheduleEvent(EVENT_STICKY_RESIN, 15000);
            }

            void JustDied(Unit* /*killer*/)
            {
                if (!GetClosestCreatureWithEntry(me, NPC_SETTHIK_WINDBLADE, 200.0f))
                    if (!GetClosestCreatureWithEntry(me, NPC_KORTHIK_REAVER, 200.0f))
                        if (pInstance)
                            if (Creature* Shekzeer = pInstance->instance->GetCreature(pInstance->GetData64(NPC_SHEKZEER)))
                                Shekzeer->AI()->DoAction(ACTION_ALL_ADD_DIED);
            }

            void UpdateAI(const uint32 diff)
            {
                if (me->HasAura(SPELL_TRAP_DAMAGES))
                    return;

                ShekzeerBuffAdds(me);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                UpdateVictim();

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_DISPATCH:
                        {
                            DoCast(SPELL_ADD_DISPATCH);
                            events.ScheduleEvent(EVENT_DISPATCH, 14000);
                            break;
                        }
                        case EVENT_SONIC_BLADE:
                        {
                            if (Unit* target = me->getVictim())
                                me->CastSpell(target, SPELL_ADD_SONIC_BLADE, true);
                            events.ScheduleEvent(EVENT_SONIC_BLADE, urand(20000, 25000));
                            break;
                        }
                        case EVENT_STICKY_RESIN:
                        {
                            float x, y, z;
                            me->GetNearPoint(me, x, y, z, 1.0f, frand(5.0f, 30.0f), frand(0.0, 2 * M_PI));

                            me->CastSpell(x, y, z, SPELL_STICKY_RESIN, false);
                            events.ScheduleEvent(EVENT_STICKY_RESIN, 20000);
                            break;
                        }
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_add_setthik_windbladeAI(creature);
        }
};

// 63591 - Kor'thik Reaver
class mob_korthik_reaver : public CreatureScript
{
    public:
        mob_korthik_reaver() : CreatureScript("mob_korthik_reaver") { }

        struct mob_korthik_reaverAI : public ScriptedAI
        {
            mob_korthik_reaverAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* pInstance;

            void Reset()
            {
                events.Reset();
                me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, EQUIP_TRASH_4);

                events.ScheduleEvent(EVENT_POISON_BOMB, 10000);
                events.ScheduleEvent(EVENT_TOXIC_SLIME, 14000);
                events.ScheduleEvent(EVENT_POISON_DRENCHED_ARMOR, 21000);
            }

            void JustDied(Unit* /*killer*/)
            {
                if (!GetClosestCreatureWithEntry(me, NPC_SETTHIK_WINDBLADE, 200.0f))
                    if (!GetClosestCreatureWithEntry(me, NPC_KORTHIK_REAVER, 200.0f))
                        if (pInstance)
                            if (Creature* Shekzeer = pInstance->instance->GetCreature(pInstance->GetData64(NPC_SHEKZEER)))
                                Shekzeer->AI()->DoAction(ACTION_ALL_ADD_DIED);
            }

            void UpdateAI(const uint32 diff)
            {
                if (me->HasAura(SPELL_TRAP_DAMAGES))
                    return;

                ShekzeerBuffAdds(me);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_POISON_BOMB:
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                me->CastSpell(target, SPELL_POISON_BOMB, true);
                            events.ScheduleEvent(EVENT_POISON_BOMB, urand(7000, 8000));
                            break;
                        }
                        case EVENT_POISON_DRENCHED_ARMOR:
                        {
                            if (Unit* target = me->getVictim())
                                me->CastSpell(target, SPELL_POISON_DRENCHED_ARMOR, true);
                            events.ScheduleEvent(EVENT_POISON_DRENCHED_ARMOR, 21000);
                            break;
                        }
                        case EVENT_TOXIC_SLIME:
                        {
                            DoCast(SPELL_TOXIC_SLIME);
                            events.ScheduleEvent(EVENT_TOXIC_SLIME, 14000);
                            break;
                        }
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_korthik_reaverAI(creature);
        }
};

// 63730 - Sticky Resin
class mob_sticky_resin : public CreatureScript
{
    public:
        mob_sticky_resin() : CreatureScript("mob_sticky_resin") { }

        struct mob_sticky_resinAI : ScriptedAI
        {
            mob_sticky_resinAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;
            uint8 growth;
            bool checkTrap;

            void Reset()
            {
                events.Reset();
                me->SetReactState(REACT_PASSIVE);
                me->AddAura(SPELL_STICKY_RESIN_VISUAL, me);
                growth = 0;
                checkTrap = false;
                events.ScheduleEvent(EVENT_CHECK_INTERRACT, 1000);
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_RESIN_GROW:
                    {
                        ++growth;
                        DoCast(SPELL_RESIN_SPURT);

                        checkTrap = false;
                        events.ScheduleEvent(EVENT_CHECK_INTERRACT, 1000);

                        // Changing into amber trap
                        if (growth >= 4)
                        {
                            me->RemoveAura(SPELL_MERGED_RESIN);
                            me->AddAura(SPELL_AMBER_TRAP, me);
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                if (events.ExecuteEvent() == EVENT_CHECK_INTERRACT)
                    checkTrap = true;

                if (!checkTrap)
                    return;

                // Interaction with players
                if (Player* player = me->SelectNearestPlayer(1.0f))
                {
                    // If we're a little resin
                    if (me->HasAura(SPELL_STICKY_RESIN_VISUAL))
                    {
                        // Player hasn't sticky aura: we put it on the player and despawn
                        if (!player->HasAura(SPELL_STICKY_DAMAGES))
                        {
                            player->AddAura(SPELL_STICKY_DAMAGES, player);
                            me->AddAura(SPELL_STICKY_RESIN_VISUAL, player);
                            me->DespawnOrUnsummon();
                        }
                        // Player already has aura: we grow
                        else
                        {
                            player->RemoveAura(SPELL_STICKY_DAMAGES);
                            player->RemoveAura(SPELL_STICKY_RESIN_VISUAL);

                            me->RemoveAura(SPELL_STICKY_RESIN_VISUAL);
                            me->AddAura(SPELL_MERGED_RESIN, me);

                            // To avoid multiple triggering on once merge
                            checkTrap = false;
                            events.ScheduleEvent(EVENT_CHECK_INTERRACT, 1000);
                        }
                    }
                    // If we're a big resin
                    else if (me->HasAura(SPELL_MERGED_RESIN))
                    {
                        player->RemoveAura(SPELL_STICKY_RESIN_VISUAL);
                        player->RemoveAura(SPELL_STICKY_DAMAGES);
                        DoAction(ACTION_RESIN_GROW);
                    }
                    // If we're an amber trap
                    else if (me->HasAura(SPELL_AMBER_TRAP))
                    {
                        me->AddAura(SPELL_TRAPPED, player);
                        me->SummonCreature(NPC_AMBER_TRAP, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ());
                        checkTrap = false;
                        me->DespawnOrUnsummon();
                    }
                }
                // No player around: if we're an amber trap, we can also trap mobs
                else
                {
                    if (me->HasAura(SPELL_AMBER_TRAP))
                    {
                        Creature* target = 0;

                        // if target isn't a Set'thik Windblade, it should be a Kor'thik Reaver
                        if (!(target = GetClosestCreatureWithEntry(me, NPC_SETTHIK_WINDBLADE, 1.0f)))
                            target = GetClosestCreatureWithEntry(me, NPC_KORTHIK_REAVER, 1.0f);

                        // There's a mob to trap
                        if (target)
                        {
                            me->AddAura(SPELL_TRAPPED, target);
                            me->SummonCreature(NPC_AMBER_TRAP, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ());
                            checkTrap = false;
                            me->DespawnOrUnsummon();
                        }
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_sticky_resinAI(creature);
        }
};

// 64351 - Amber trap
class mob_amber_trap : public CreatureScript
{
    public:
        mob_amber_trap() : CreatureScript("mob_amber_trap") { }

        struct mob_amber_trapAI : public ScriptedAI
        {
            mob_amber_trapAI(Creature* creature) : ScriptedAI(creature) { }

            uint64 targetGuid;

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);
                me->AddAura(SPELL_AMBER_TRAP, me);
                me->SetDisplayId(43958);

                // --- Retrieving trapped unit ---
                targetGuid = 0;
                Unit* target = 0;
                // Among the Kor'thik Reavers
                std::list<Creature*> addList;
                GetCreatureListWithEntryInGrid(addList, me, NPC_KORTHIK_REAVER, 1.0f);

                if (!addList.empty())
                    for (Creature* add : addList)
                        if (add->HasAura(SPELL_TRAPPED))
                            target = add;

                // Among the Set'thik Windblades
                addList.clear();
                if (!target)
                {
                    GetCreatureListWithEntryInGrid(addList, me, NPC_SETTHIK_WINDBLADE, 1.0f);
                    if (!addList.empty())
                        for (Creature* add : addList)
                            if (add->HasAura(SPELL_TRAPPED))
                                target = add;
                }

                // if the trapped unit is a creature, call the alive windblade to free him
                if (target)
                {
                    addList.clear();
                    GetCreatureListWithEntryInGrid(addList, me, NPC_SETTHIK_WINDBLADE, 200.0f);

                    if (!addList.empty())
                    {
                        for (Creature* setthik : addList)
                        {
                            if (setthik->isAlive())
                            {
                                if (!setthik->getVictim() || setthik->getVictim()->GetEntry() != me->GetEntry())
                                {
                                    setthik->SetInCombatWith(me);
                                    setthik->AI()->AttackStart(me);
                                }
                            }
                        }
                    }
                }

                // If the trapped unit isn't a mob, we look among the players
                if (!target)
                {
                    std::list<Player*> playerList;
                    GetPlayerListInGrid(playerList, me, 1.0f);

                    for (Player* player : playerList)
                        if (player->HasAura(SPELL_TRAPPED))
                            target = player;
                }

                if (target)
                {
                    targetGuid = target->GetGUID();
                    me->AddAura(SPELL_FREEZE_ANIM, target);
                    me->AddAura(SPELL_TRAP_DAMAGES, target);
                }
                else
                    me->DespawnOrUnsummon();
            }

            void JustDied(Unit* /*killer*/)
            {
                if (targetGuid)
                    if (Unit* target = ObjectAccessor::FindUnit(targetGuid))
                        target->RemoveAura(SPELL_TRAPPED);
            }

            void UpdateAI(const uint32 diff)
            {
                if (Unit* target = ObjectAccessor::FindUnit(targetGuid))
                {
                    if (target->isDead())
                    {
                        target->RemoveAura(SPELL_TRAP_DAMAGES);
                        target->RemoveAura(SPELL_FREEZE_ANIM);
                        me->DespawnOrUnsummon();
                    }
                }
                else
                    me->DespawnOrUnsummon();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_amber_trapAI(creature);
        }
};

// 62847 - Dissonance Field
class mob_dissonance_field : public CreatureScript
{
    public:
        mob_dissonance_field() : CreatureScript("mob_dissonance_field") { }

        struct mob_dissonance_fieldAI : public ScriptedAI
        {
            mob_dissonance_fieldAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                events.Reset();
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_NON_ATTACKABLE);
                me->SetReactState(REACT_PASSIVE);
                events.ScheduleEvent(EVENT_CHECK_CAST, 1000);

                std::list<Player*> playerList;
                GetPlayerListInGrid(playerList, me, 200.0f);

                for (Player* plr : playerList)
                    if (!plr->HasAura(SPELL_DISSONANCE_SPELLS))
                        me->AddAura(SPELL_DISSONANCE_SPELLS, plr);
            }

            // Can't be wounded directly by player attacks
            void DamageTaken(Unit* attacker, uint32 &damage)
            {
                if (attacker->GetTypeId() == TYPEID_PLAYER || attacker->GetEntry() == me->GetEntry())
                    damage = 0;
            }

            void IsSummonedBy(Unit* summoner)
            {
                if (summoner)
                    me->CastSpell(summoner, SPELL_DISSONANCE_FIELD_AURA, false);
            }

            void UpdateAI(const uint32 diff)
            {
                if (me->isDead())
                    return;

                events.Update(diff);

                if (events.ExecuteEvent() == EVENT_CHECK_CAST)
                {
                    std::list<Player*> playerList;
                    GetPlayerListInGrid(playerList, me, 200.0f);

                    if (!playerList.empty())
                    {
                        uint8 dmg = 0;
                        for (Player* plr : playerList)
                        {
                            if (plr->GetCurrentSpell(CURRENT_GENERIC_SPELL)    || 
                                plr->GetCurrentSpell(CURRENT_MELEE_SPELL)      || 
                                plr->GetCurrentSpell(CURRENT_CHANNELED_SPELL)  ||
                                plr->GetCurrentSpell(CURRENT_AUTOREPEAT_SPELL) ||
                                plr->HasUnitState(UNIT_STATE_CASTING))
                            {
                                ++dmg;
                                if (plr->GetDistance2d(me) < 7.0f)
                                    ++dmg;
                            }

                            if (plr->GetDistance2d(me) < 7.0f && plr->HasAura(SPELL_CRY_OF_TERROR))
                                ++dmg;
                        }
                        me->ModifyHealth(-dmg);
                        if (me->GetHealth() <= 0)
                        {
                            DoCast(SPELL_SONIC_DISCHARGE);
                            me->DespawnOrUnsummon();
                        }
                    }
                    events.ScheduleEvent(EVENT_CHECK_CAST, 1000);
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_dissonance_fieldAI(creature);
        }
};

// 63942 - Sha of Fear
class mob_sha_of_fear : public CreatureScript
{
    public:
        mob_sha_of_fear() : CreatureScript("mob_sha_of_fear") { }

        struct mob_sha_of_fearAI : public ScriptedAI
        {
            mob_sha_of_fearAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
                me->SetReactState(REACT_PASSIVE);
                if (me->isDead())
                    me->Respawn();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
                me->SetDisplayId(DISPLAYID_INVISIBLE);
            }

            void DamageTaken(Unit* /*attacker*/, uint32 &damage)
            {
                damage =  0;
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    // Entering in phase 1 : appearing and growing
                    case ACTION_COMBAT:
                    {
                        me->SetDisplayId(me->GetNativeDisplayId());
                        DoCast(SPELL_SHA_OF_FEAR);
                        Talk(SAY_LAST_PHASE);
                        break;
                    }
                    // Entering in phase 2 : disappearing
                    case ACTION_RETREAT:
                    {
                        me->RemoveAura(SPELL_SHA_OF_FEAR);
                        events.ScheduleEvent(EVENT_CLOSE_CHAMBER, 2000);
                    }
                    // Entering in phase 3 : cast Ultimate Corruption on Shek'zeer
                    case ACTION_LAST_PHASE:
                    {
                        if (Creature* shekzeer = pInstance->instance->GetCreature(pInstance->GetData64(NPC_SHEKZEER)))
                            me->CastSpell(shekzeer, SPELL_ULTIMATE_CORRUPTION, true);
                        break;
                    }
                    // Shek'zeer defeated, fleeing away
                    case ACTION_WITHDRAW:
                    {
                        Talk(SAY_DEAD_QUEEN);
                        events.ScheduleEvent(EVENT_LEAVING, 3000);
                        break;
                    }
                    default:
                        break;
                }
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                switch (id)
                {
                    // Reaching the center of the room
                    case 1:
                    {
                        me->SetCanFly(true);
                        me->SetDisableGravity(true);
                        me->SetByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_HOVER);

                        me->GetMotionMaster()->MoveTakeoff(2, highPos);
                        break;
                    }
                    // Flying to the roof and breaking the ceiling
                    case 2:
                    {
                        if (me->GetPositionZ() >= 750.0f)
                        {
                            if (GameObject* ceil = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_MANTID_QUEEN_CEIL)))
                                ceil->SetGoState(GO_STATE_ACTIVE);

                            me->DespawnOrUnsummon(1000);
                        }
                    }
                }
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CLOSE_CHAMBER:
                        {
                            me->SetDisplayId(DISPLAYID_INVISIBLE);
                            break;
                        }
                        case EVENT_LEAVING:
                        {
                            me->GetMotionMaster()->MovePoint(1, leavingPos);
                            Talk(SAY_LEAVING);
                            break;
                        }
                        default:
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_sha_of_fearAI(creature);
        }
};

// 123707 - Eyes of the empress
class spell_eyes_of_the_empress : public SpellScriptLoader
{
    public:
        spell_eyes_of_the_empress() : SpellScriptLoader("spell_eyes_of_the_empress") { }

        class spell_eyes_of_the_empress_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_eyes_of_the_empress_SpellScript);

            void Transform(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetHitUnit())
                {
                    AuraPtr empressAura = target->GetAura(SPELL_EYES_OF_THE_EMPRESS);
                    if (empressAura->GetStackAmount() == 5)
                    {
                        if (Unit* caster = GetCaster())
                        {
                            target->CastSpell(target, SPELL_SERVANT_OF_THE_EMPRESS, false);
                            if (Creature* shekzeer = target->GetInstanceScript()->instance->GetCreature(target->GetInstanceScript()->GetData64(NPC_SHEKZEER)))
                                shekzeer->AI()->Talk(SAY_SPELL_SERVANT);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_eyes_of_the_empress_SpellScript::Transform, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_eyes_of_the_empress_SpellScript();
        }
};

// 123792 - Cry of Terror
class spell_cry_of_terror : public SpellScriptLoader
{
    public:
        spell_cry_of_terror() : SpellScriptLoader("spell_cry_of_terror") { }

        class spell_cry_of_terror_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_cry_of_terror_SpellScript);

            void Cancel(std::list<WorldObject*>& targets)
            {
                WorldObject* uniqueTarget = 0;
                std::list<WorldObject*>::iterator itr = targets.begin();

                // Looking if one of the targets affected by cry of terror is in a dissonance field
                while (!uniqueTarget && itr != targets.end())
                {
                    if (Creature* dissonanceField = GetClosestCreatureWithEntry((*itr), NPC_DISSONANCE_FIELD, 5.0f))
                        if ((*itr)->ToUnit()->HasAura(SPELL_CRY_OF_TERROR))
                            uniqueTarget = *itr;

                    ++itr;
                }

                // If so, only this target will be damaged
                if (uniqueTarget)
                {
                    targets.clear();
                    targets.push_back(uniqueTarget);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_cry_of_terror_SpellScript::Cancel, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_cry_of_terror_SpellScript();
        }
};

// 124843 - Amassing Darkness
class spell_amassing_darkness : public SpellScriptLoader
{
    public:
        spell_amassing_darkness() : SpellScriptLoader("spell_amassing_darkness") { }

        class spell_amassing_darkness_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_amassing_darkness_SpellScript);

            void ScriptEffect(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    std::list<Player*> playerList;
                    std::list<Player*> extendList;
                    GetPlayerListInGrid(playerList, caster, 200.0f);

                    for (Player* player : playerList)
                    {
                        // Cast damage spell on player who are "marked" with amassing darkness aura
                        if (player->HasAura(SPELL_AMASSING_DARKNESS))
                            caster->CastSpell(player, SPELL_AMASSING_DMG, false);
                        // If player hasn't amassing darkness aura, he could be selected to receive it
                        else
                            extendList.push_back(player);
                    }

                    if (!extendList.empty())
                    {
                        bool searching = true;
                        std::list<Player*>::iterator itr = extendList.begin();

                        while (searching)
                        {
                            if (urand(0, 1))
                            {
                                caster->CastSpell(*itr, SPELL_AMASSING_DARKNESS, true);
                                searching = false;
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectLaunch += SpellEffectFn(spell_amassing_darkness_SpellScript::ScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_amassing_darkness_SpellScript();
        }
};

// 123723 - Servant of the Empress
class spell_servant_of_the_empress : public SpellScriptLoader
{
    public:
        spell_servant_of_the_empress() : SpellScriptLoader("spell_servant_of_the_empress") { }

        class spell_servant_of_the_empress_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_servant_of_the_empress_SpellScript);

            void Transform(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetHitUnit())
                    target->CastSpell(target, SPELL_CONVERT_SERVANT, false);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_servant_of_the_empress_SpellScript::Transform, EFFECT_1, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_servant_of_the_empress_SpellScript();
        }
};

// 124845 - Calamity
class spell_calamity : public SpellScriptLoader
{
    public:
        spell_calamity() : SpellScriptLoader("spell_calamity") { }

        class spell_calamity_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_calamity_SpellScript);

            void DivideLife()
            {
                if (Unit* target = GetHitUnit())
                    SetHitDamage(int32(target->GetHealth() * 0.5f));
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_calamity_SpellScript::DivideLife);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_calamity_SpellScript();
        }
};

void AddSC_boss_shekzeer()
{
    new boss_shekzeer();                // 62837 - Shek'zeer
    new mob_add_setthik_windblade();    // 63589 - Set'thik Windblade
    new mob_korthik_reaver();           // 63591 - Kor'thik Reaver
    new mob_sticky_resin();             // 63730 - Sticky Resin
    new mob_amber_trap();               // 64351 - Amber trap
    new mob_dissonance_field();         // 62847 - Dissonance Field
    new mob_sha_of_fear();              // 63942 - Sha of Fear
    new spell_eyes_of_the_empress();    // 123707 - Eyes of the empress
    new spell_cry_of_terror();          // 123792 - Cry of terror
    new spell_servant_of_the_empress(); // 123723 - Servant of the empress
    new spell_amassing_darkness();      // 124843 - Amassing Darkness
    new spell_calamity();               // 124845 - Calamity
}