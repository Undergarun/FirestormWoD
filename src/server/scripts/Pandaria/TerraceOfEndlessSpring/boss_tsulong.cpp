////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "GameObjectAI.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "terrace_of_endless_spring.h"

enum eTsulongEvents
{
    EVENT_NONE,
    EVENT_FLY,
    EVENT_WAYPOINT_FIRST,
    EVENT_WAYPOINT_SECOND,
    EVENT_SWITCH_TO_NIGHT_PHASE,
    EVENT_SWITCH_TO_DAY_PHASE,
    EVENT_SPAWN_SUNBEAM,
    EVENT_SHADOW_BREATH,
    EVENT_NIGHTMARES,
    EVENT_DARK_OF_NIGHT,    //Heroic
    EVENT_UP_ENERGY,
    EVENT_SUN_BREATH,
    EVENT_SPAWN_EMBODIED_TERROR,
    EVENT_UNSTABLE_SHA,
    EVENT_EMBODIED_TERROR_MOVE,
    EVENT_DARK_OF_NIGHT_MOVE,
    EVENT_UNSTABLE_SHA_MOVE
};

enum eTsulongSpells
{
    // Tsulong
    SPELL_DREAD_SHADOWS        = 122767,
    SPELL_DREAD_SHADOWS_DEBUFF = 122768,
    SPELL_SUNBEAM_DUMMY        = 122782,
    SPELL_SUNBEAM_PROTECTION   = 122789,
    SPELL_SHADOW_BREATH        = 122752,
    SPELL_NIGHTMARES           = 122770,
    SPELL_SPAWN_DARK_OF_NIGHT  = 123739,
    SPELL_TRIGGER_LIGHT_OF_DAY = 123816,
    SPELL_DAY_PHASE            = 122453,
    SPELL_SUN_BREATH           = 122855,
    SPELL_BATHED_IN_LIGHT      = 122858,
    SPELL_SUMMON_UNSTABLE_SHA  = 122953,
    SPELL_BERSERK              =  26662,

    // The dark of the night
    SPELL_BUMP_DARK_OF_NIGHT   = 130013,
    SPELL_VISUAL_DARK_OF_NIGHT = 123740,
    SPELL_VISUAL_LINK_TO_BEAM  = 137567,

    // The light of the day
    SPELL_BUFF_LIGHT_OF_DAY    = 123716,

    // The embodied terror
    SPELL_TERRORIZE_PLAYER     = 123011,
    SPELL_TERRORIZE_TSULONG    = 123012,
    SPELL_SUMMON_TINY_TERROR   = 123027,

    // Tiny terror
    SPELL_FRIGHT               = 123036,

    // Unstable sha
    SPELL_BOLT                 = 122881,
    SPELL_INSTABILITY          = 123697,

    SPELL_TSULONG_BONUS        = 132201
};

enum eTsulongTimers
{
    TIMER_FIRST_WAYPOINT  =  5000, // 5 secs for test, live : 120 000
    TIMER_SHADOW_BREATH   = 25000,
    TIMER_NIGHTMARES      = 11600,
    TIMER_DARK_OF_NIGHT   = 30000,
    TIMER_UP_ENERGY       =  1200,
    TIMER_SUN_BREATH      = 29000,
    TIMER_EMBODIED_TERROR = 41000,
    TIMER_TERRORIZE       = 13500,
    TIMER_FRIGHT          =  6000,
    TIMER_UNSTABLE_SHA    = 18000,
    TIMER_BOLT            =  2000
};

enum eTsulongPhase
{
    PHASE_NONE,
    PHASE_FLY,
    PHASE_DAY,
    PHASE_NIGHT
};

enum eTsulongWaypoints
{
    WAYPOINT_FIRST         = 10001,
    WAYPOINT_SECOND        = 10002,
    WAYPOINT_TO_DAY_PHASE  = 10003
};

enum eTsulongDisplay
{
    DISPLAY_TSULON_NIGHT = 42532,
    DISPLAY_TSULON_DAY   = 42533
};

enum eTsulongActions
{
    ACTION_SPAWN_SUNBEAM = 3
};

enum eTsulongCreatures
{
    SUNBEAM_DUMMY_ENTRY     = 62849,
    EMBODIED_TERROR         = 62969,
    UNSTABLE_SHA_DUMMY      = 62962,
    TINY_TERROR             = 62977,
    NPC_DARK_OF_NIGHT       = 63346
};

enum eTsulongTexts
{
    VO_TES_SERPENT_AGGRO            = 0,
    VO_TES_SERPENT_DEATH            = 1,
    VO_TES_SERPENT_EVENT_DAYTONIGHT = 2,
    VO_TES_SERPENT_EVENT_NIGHTTODAY = 3,
    VO_TES_SERPENT_INTRO            = 4,
    VO_TES_SERPENT_SLAY_NIGHT       = 5,
    VO_TES_SERPENT_SLAY_DAY         = 6,
    VO_TES_SERPENT_SPELL_NIGGHTMARE = 7
};

// 62442 - Tsulong
class boss_tsulong : public CreatureScript
{
    public:
        boss_tsulong() : CreatureScript("boss_tsulong") { }

        struct boss_tsulongAI : public BossAI
        {
            boss_tsulongAI(Creature* creature) : BossAI(creature, DATA_TSULONG)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            uint8 phase;
            bool firstSpecialEnabled;
            bool secondSpecialEnabled;
            bool inFly;
            bool start;
            bool needToLeave;
            uint32 berserkTimer;
            uint32 leaveTimer;

            void Reset()
            {
                me->ResetLootMode();
                events.Reset();
                summons.DespawnAll();
                DespawnAllSunbeams();
                DespawnAllTinyTerror();

                inFly = false;
                start = false;
                needToLeave = false;
                leaveTimer = 0;
                berserkTimer = 60000 * 8;

                me->SetDisableGravity(true);
                me->ReenableEvadeMode();
                me->SetCanFly(true);
                me->RemoveAurasDueToSpell(SPELL_DREAD_SHADOWS);
                me->RemoveAurasDueToSpell(SPELL_DAY_PHASE);
                me->ClearUnitState(UNIT_STATE_ROOT);
                me->setPowerType(POWER_ENERGY);
                me->SetPower(POWER_ENERGY, 0);
                me->SetMaxPower(POWER_ENERGY, 100);
                me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_REGENERATE_POWER);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                phase = PHASE_NONE;
                events.SetPhase(PHASE_NONE);

                if (pInstance)
                {
                    if (pInstance->GetBossState(DATA_TSULONG) == DONE)
                    {
                        pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DREAD_SHADOWS_DEBUFF);
                        pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SHADOW_BREATH);
                        pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_NIGHT_PHASE_EFFECT);

                        needToLeave = true;
                        leaveTimer = 10000;
                        me->SetDisplayId(DISPLAY_TSULON_DAY);
                        me->setFaction(2104);
                        phase = PHASE_NONE;
                        events.SetPhase(PHASE_NONE);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                    }
                    else if (pInstance->GetBossState(DATA_PROTECTORS) == DONE)
                    {
                        me->SetDisplayId(DISPLAY_TSULON_NIGHT);
                        me->setFaction(14);
                        me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                        me->SetHomePosition(-1017.841f, -3049.621f, 12.823f, 4.72f);
                        me->GetMotionMaster()->MoveTargetedHome();
                    }
                    else
                    {
                        me->SetDisplayId(DISPLAY_TSULON_DAY);
                        me->setFaction(2104);
                        me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 35);
                    }
                }
            }

            void JustReachedHome()
            {
                _JustReachedHome();

                if (pInstance && pInstance->GetBossState(DATA_TSULONG) != DONE)
                    pInstance->SetBossState(DATA_TSULONG, FAIL);
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    DoZoneInCombat();
                }

                me->DisableEvadeMode();
                me->DisableHealthRegen();

                start = true;
                phase = PHASE_NIGHT;
                events.SetPhase(PHASE_NIGHT);
                events.ScheduleEvent(EVENT_SWITCH_TO_NIGHT_PHASE, 0, 0, PHASE_NIGHT);

                Talk(VO_TES_SERPENT_AGGRO);

                if (pInstance)
                    pInstance->SetBossState(DATA_TSULONG, IN_PROGRESS);
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
                if (who->GetTypeId() != TYPEID_PLAYER)
                    return;

                if (phase == PHASE_NIGHT)
                {
                    who->RemoveAura(SPELL_NIGHT_PHASE_EFFECT);
                    Talk(VO_TES_SERPENT_SLAY_NIGHT);
                }
                else
                    Talk(VO_TES_SERPENT_SLAY_DAY);
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_START_TSULONG_WAYPOINT)
                {
                    phase = PHASE_FLY;
                    events.SetPhase(phase);
                    events.ScheduleEvent(EVENT_FLY, 5000, 0, phase);
                    Talk(VO_TES_SERPENT_INTRO);
                }

                if (action == ACTION_SPAWN_SUNBEAM)
                    events.ScheduleEvent(EVENT_SPAWN_SUNBEAM, 0, 0, PHASE_NIGHT);
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                switch (id)
                {
                    case WAYPOINT_FIRST:
                        events.ScheduleEvent(EVENT_WAYPOINT_FIRST, 0, 0, PHASE_FLY);
                        break;
                    case WAYPOINT_SECOND:
                        events.ScheduleEvent(EVENT_WAYPOINT_SECOND, 0, 0, PHASE_FLY);
                        break;
                    case WAYPOINT_TO_DAY_PHASE:
                        me->SetOrientation(4.7f);
                        me->SetFacingTo(4.7f);
                        me->AddUnitState(UNIT_STATE_ROOT);
                        break;
                    default:
                        break;
                }
            }

            void RegeneratePower(Powers /*power*/, int32& value)
            {
                value = 0;
            }

            void DamageTaken(Unit* /*doneBy*/, uint32 &damage, const SpellInfo* /*p_SpellInfo*/)
            {
                if (pInstance && pInstance->GetBossState(DATA_TSULONG) == DONE)
                {
                    damage = 0;
                    return;
                }

                if (phase == PHASE_DAY)
                {
                    uint32 health = me->GetHealth();
                    if (health == 1)
                    {
                        damage = 0;
                        return;
                    }

                    if (damage >= me->GetHealth())
                        damage = me->GetHealth() - 1;
                }
                
                if (phase == PHASE_NIGHT)
                {
                    if (damage >= me->GetHealth())
                    {
                        damage = 0;
                        EndOfFight();
                    }
                }
            }

            void EndOfFight()
            {
                Talk(VO_TES_SERPENT_DEATH);

                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->SetBossState(DATA_TSULONG, DONE);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DREAD_SHADOWS);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DREAD_SHADOWS_DEBUFF);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_NIGHT_PHASE_EFFECT);
                }

                me->RemoveAllAreasTrigger();
                me->RemoveAllAuras();
                me->ReenableHealthRegen();
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->ReenableEvadeMode();
                me->ReenableHealthRegen();
                me->CombatStop();
                EnterEvadeMode();
                me->GetMotionMaster()->MoveTargetedHome();
                me->ResetLootMode();
                events.Reset();
                summons.DespawnAll();

                Map::PlayerList const& playerList = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
                    if (Player* player = itr->getSource())
                        player->CombatStop();

                switch (me->GetMap()->GetSpawnMode())
                {
                    case Difficulty::Difficulty10N:
                         me->SummonGameObject(CACHE_OF_TSULONG_10_NM, -1018.64f, -2996.85f, 12.30f, 4.75f, 0.0f, 0.0f, 0.0f, 0.0f, 0);
                        break;
                    case Difficulty::Difficulty25N:
                         me->SummonGameObject(CACHE_OF_TSULONG_25_NM, -1018.64f, -2996.85f, 12.30f, 4.75f, 0.0f, 0.0f, 0.0f, 0.0f, 0);
                        break;
                    case Difficulty::Difficulty10HC:
                         me->SummonGameObject(CACHE_OF_TSULONG_10_HM, -1018.64f, -2996.85f, 12.30f, 4.75f, 0.0f, 0.0f, 0.0f, 0.0f, 0);
                        break;
                    case Difficulty::Difficulty25HC:
                         me->SummonGameObject(CACHE_OF_TSULONG_25_HM, -1018.64f, -2996.85f, 12.30f, 4.75f, 0.0f, 0.0f, 0.0f, 0.0f, 0);
                        break;
                    default:
                        break;
                }

                Map::PlayerList const& l_PlrList = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator l_Itr = l_PlrList.begin(); l_Itr != l_PlrList.end(); ++l_Itr)
                {
                    if (Player* l_Player = l_Itr->getSource())
                    {
                        // Combat stop, avoid fight bug
                        l_Player->CombatStop();

                        // Tsulong bonus
                        me->CastSpell(l_Player, SPELL_TSULONG_BONUS, true);

                        // Valor points
                        l_Player->ModifyCurrency(CURRENCY_TYPE_VALOR_POINTS, 40);
                    }
                }

                if (me->GetMap()->IsLFR())
                {
                    me->SetLootRecipient(NULL);
                    Player* l_Player = me->GetMap()->GetPlayers().begin()->getSource();
                    if (l_Player && l_Player->GetGroup())
                        sLFGMgr->AutomaticLootAssignation(me, l_Player->GetGroup());
                }
            }

            void CheckCombatState()
            {
                if (pInstance && pInstance->IsWipe())
                {
                    me->ReenableEvadeMode();
                    me->ReenableHealthRegen();
                    me->CombatStop();
                    EnterEvadeMode();
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_NIGHT_PHASE_EFFECT);
                }
            }

            void DespawnAllTinyTerror()
            {
                std::list<Creature*> tinyTerrors;
                me->GetCreatureListWithEntryInGrid(tinyTerrors, TINY_TERROR, 200.0f);
                for (auto itr : tinyTerrors)
                    itr->DespawnOrUnsummon();
            }

            void DespawnAllSunbeams()
            {
                std::list<Creature*> sunbeams;
                me->GetCreatureListWithEntryInGrid(sunbeams, SUNBEAM_DUMMY_ENTRY, 200.0f);
                for (auto itr : sunbeams)
                    itr->DespawnOrUnsummon();
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                if (needToLeave)
                {
                    if (leaveTimer <= diff)
                    {
                        me->GetMotionMaster()->MovePoint(1, -642.66f, -2852.23f, 207.57f);
                        needToLeave = false;
                    }
                    else
                        leaveTimer -= diff;
                }

                if (phase == PHASE_DAY && me->GetHealth() == me->GetMaxHealth())
                {
                    EndOfFight();
                    return;
                }

                if (phase == PHASE_DAY || phase == PHASE_NIGHT)
                {
                    if (berserkTimer <= diff)
                    {
                        DoCast(SPELL_BERSERK);
                        berserkTimer = 8*60000;
                    }
                    else 
                        berserkTimer -= diff;
                }

                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING) || inFly)
                {
                    if (pInstance && pInstance->GetData(SPELL_RITUAL_OF_PURIFICATION) == false)
                        me->RemoveAura(SPELL_RITUAL_OF_PURIFICATION);

                    if (phase == PHASE_FLY)
                    {
                        switch (events.ExecuteEvent())
                        {
                            case EVENT_FLY:
                                me->setFaction(14);
                                me->SetReactState(REACT_PASSIVE);
                                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                                me->SetDisplayId(DISPLAY_TSULON_NIGHT);
                                me->GetMotionMaster()->MovePoint(WAYPOINT_FIRST, -1018.10f, -2947.431f, 50.12f);
                                inFly = true;
                                break;
                            case EVENT_WAYPOINT_FIRST:
                                me->GetMotionMaster()->Clear();
                                me->GetMotionMaster()->MovePoint(WAYPOINT_SECOND, -1017.841f, -3049.621f, 12.823f);
                                break;
                            case EVENT_WAYPOINT_SECOND:
                                me->SetHomePosition(-1017.841f, -3049.621f, 12.823f, 4.72f);
                                me->SetReactState(REACT_AGGRESSIVE);
                                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                                inFly = false;
                                events.SetPhase(PHASE_NONE);
                                phase = PHASE_NONE;
                                break;
                            default:
                                break;
                        }
                    }
                    else if (phase != PHASE_NONE)
                        CheckCombatState();
                    return;
                }

                if (phase == PHASE_NIGHT)
                {
                    CheckCombatState();
                    // Check if all players have night visual aura
                    Map::PlayerList const& playerList = me->GetMap()->GetPlayers();
                    for (Map::PlayerList::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
                        if (Player* player = itr->getSource())
                            if (!player->HasAura(SPELL_NIGHT_PHASE_EFFECT) && player->isAlive())
                                me->AddAura(SPELL_NIGHT_PHASE_EFFECT, player);

                    switch (events.ExecuteEvent())
                    {
                        case EVENT_SWITCH_TO_NIGHT_PHASE:
                            if (!start)
                                Talk(VO_TES_SERPENT_EVENT_DAYTONIGHT);
                            else
                                start = false;
                            me->SetDisplayId(DISPLAY_TSULON_NIGHT);
                            me->RemoveAurasDueToSpell(SPELL_DAY_PHASE);
                            me->RemoveAurasDueToSpell(SPELL_TERRORIZE_TSULONG);
                            if (IsHeroic())
                                me->RemoveAurasDueToSpell(SPELL_TRIGGER_LIGHT_OF_DAY);
                            me->ClearUnitState(UNIT_STATE_ROOT);
                            me->setFaction(14);
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->CastSpell(me, SPELL_DREAD_SHADOWS, true);
                            me->SetPower(POWER_ENERGY, 0);
                            events.RescheduleEvent(EVENT_SPAWN_SUNBEAM, 2000, 0, PHASE_NIGHT);
                            events.RescheduleEvent(EVENT_SHADOW_BREATH, TIMER_SHADOW_BREATH, 0, PHASE_NIGHT);
                            events.RescheduleEvent(EVENT_NIGHTMARES, TIMER_NIGHTMARES, 0, PHASE_NIGHT);
                            if (me->GetMap()->IsHeroic())
                                events.RescheduleEvent(EVENT_DARK_OF_NIGHT, TIMER_DARK_OF_NIGHT, 0, PHASE_NIGHT);
                            events.RescheduleEvent(EVENT_UP_ENERGY, TIMER_UP_ENERGY);
                            if (instance)
                                instance->DoAddAuraOnPlayers(SPELL_NIGHT_PHASE_EFFECT);
                            break;
                        case EVENT_SPAWN_SUNBEAM:
                            Position pos;
                            me->GetRandomNearPosition(pos, 30.0f);
                            me->SummonCreature(SUNBEAM_DUMMY_ENTRY, pos);
                            break;
                        case EVENT_SHADOW_BREATH:
                            me->CastSpell(SelectTarget(SELECT_TARGET_TOPAGGRO), SPELL_SHADOW_BREATH, false);
                            events.ScheduleEvent(EVENT_SHADOW_BREATH, TIMER_SHADOW_BREATH, 0, PHASE_NIGHT);
                            break;
                        case EVENT_NIGHTMARES:
                            if ((rand() % 10) > 5)
                                Talk(VO_TES_SERPENT_SPELL_NIGGHTMARE);
                            me->CastSpell(SelectTarget(SELECT_TARGET_RANDOM), SPELL_NIGHTMARES, false);
                            events.ScheduleEvent(EVENT_NIGHTMARES, TIMER_NIGHTMARES, 0, PHASE_NIGHT);
                            break;
                        case EVENT_UP_ENERGY:
                            if (me->GetPower(POWER_ENERGY) == 100)
                            {
                                me->SetPower(POWER_ENERGY, 0);
                                events.SetPhase(PHASE_DAY);
                                phase = PHASE_DAY;
                                me->SetHealth(std::max(1, int(me->GetMaxHealth() - me->GetHealth())));
                                events.ScheduleEvent(EVENT_SWITCH_TO_DAY_PHASE, 0, 0, PHASE_DAY);
                            }
                            else
                            {
                                me->ModifyPower(POWER_ENERGY, 1);
                                events.ScheduleEvent(EVENT_UP_ENERGY, TIMER_UP_ENERGY);
                            }
                            break;
                        default:
                            break;
                    }
                    DoMeleeAttackIfReady();
                }

                if (phase == PHASE_DAY)
                {
                    switch (events.ExecuteEvent())
                    {
                        case EVENT_SWITCH_TO_DAY_PHASE:
                            Talk(VO_TES_SERPENT_EVENT_NIGHTTODAY);
                            events.RescheduleEvent(EVENT_UP_ENERGY, TIMER_UP_ENERGY);
                            me->SetPower(POWER_ENERGY, 0);
                            me->RemoveAurasDueToSpell(SPELL_DREAD_SHADOWS);
                            DespawnAllSunbeams();
                            me->setFaction(2104);
                            me->CastSpell(me, SPELL_DAY_PHASE, false);
                            me->SetDisplayId(DISPLAY_TSULON_DAY);
                            me->getThreatManager().resetAllAggro();
                            me->SetReactState(REACT_PASSIVE);
                            me->AttackStop();
                            me->GetMotionMaster()->MovePoint(WAYPOINT_TO_DAY_PHASE, -1017.83f, -3040.70f, 12.823f);
                            events.RescheduleEvent(EVENT_SUN_BREATH, TIMER_SUN_BREATH, 0, PHASE_DAY);
                            events.RescheduleEvent(EVENT_SPAWN_EMBODIED_TERROR, TIMER_EMBODIED_TERROR, 0, PHASE_DAY);
                            events.RescheduleEvent(EVENT_UNSTABLE_SHA, TIMER_UNSTABLE_SHA, 0, PHASE_DAY);
                            if (IsHeroic())
                                me->CastSpell(me, SPELL_TRIGGER_LIGHT_OF_DAY, false);
                            if (instance)
                                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_NIGHT_PHASE_EFFECT);
                            break;
                        case EVENT_SUN_BREATH:
                            me->CastSpell(me, SPELL_SUN_BREATH, false);
                            events.ScheduleEvent(EVENT_SUN_BREATH, TIMER_SUN_BREATH, 0, PHASE_DAY);
                            break;
                        case EVENT_SPAWN_EMBODIED_TERROR:
                            Position pos;
                            me->GetRandomNearPosition(pos, 45.0f);
                            me->SummonCreature(EMBODIED_TERROR, pos);
                            events.ScheduleEvent(EVENT_SPAWN_EMBODIED_TERROR, TIMER_EMBODIED_TERROR, 0, PHASE_DAY);
                            break;
                        case EVENT_UNSTABLE_SHA:
                        {
                            std::list<Creature*> dummys;
                            std::vector<Creature*> dummysRands;
                            me->GetCreatureListWithEntryInGrid(dummys, UNSTABLE_SHA_DUMMY, 200.0f);
                            for (auto itr : dummys)
                                dummysRands.push_back(itr);
                            if (dummysRands.size())
                            {
                                std::random_shuffle(dummysRands.begin(), dummysRands.end());
                                for (uint32 i = 0; i < (dummysRands.size() < 3 ? dummysRands.size() : 3); i++)
                                    me->CastSpell(dummysRands[i], SPELL_SUMMON_UNSTABLE_SHA, false);
                            }
                            events.ScheduleEvent(EVENT_UNSTABLE_SHA, TIMER_UNSTABLE_SHA, 0, PHASE_DAY);
                            break;
                        }
                        default:
                            break;
                    }

                    CheckCombatState();
                    if (me->GetPower(POWER_ENERGY) == 100)
                    {
                        me->SetPower(POWER_ENERGY, 0);
                        events.SetPhase(PHASE_NIGHT);
                        phase = PHASE_NIGHT;
                        me->SetHealth(me->GetMaxHealth() - me->GetHealth());
                        events.ScheduleEvent(EVENT_SWITCH_TO_NIGHT_PHASE, 0, 0, PHASE_NIGHT);
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_tsulongAI(creature);
        }
};

// 62849 - Sunbeam
class npc_sunbeam : public CreatureScript
{
    public:
        npc_sunbeam() : CreatureScript("npc_sunbeam") { }

        struct npc_sunbeamAI : public ScriptedAI
        {
            InstanceScript* pInstance;
            EventMap events;
            std::list<uint64> darkList;
            bool despawn;

            npc_sunbeamAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
                despawn = false;
                creature->SetObjectScale(5.0f);
                creature->SetReactState(REACT_PASSIVE);
                creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE|UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE);
                creature->CastSpell(creature, SPELL_SUNBEAM_DUMMY, true);
                darkList.clear();
            }

            void Reset()
            {
                events.Reset();
                if (IsHeroic())
                    events.ScheduleEvent(EVENT_DARK_OF_NIGHT, 8000);
            }

            void DespawnDark()
            {
                if (!darkList.empty())
                    for (uint64 darkGuid : darkList)
                        if (Creature* dark = me->GetMap()->GetCreature(darkGuid))
                            dark->DespawnOrUnsummon();
            }

            void Despawn()
            {
                despawn = true;

                if (pInstance)
                {
                    if (Creature* tsulong = pInstance->instance->GetCreature(pInstance->GetData64(NPC_TSULONG)))
                        tsulong->AI()->DoAction(ACTION_SPAWN_SUNBEAM);

                    if (IsHeroic())
                        DespawnDark();
                }

                me->DespawnOrUnsummon(1000);
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                if (IsHeroic())
                    DespawnDark();
                Despawn();
            }

            void UpdateAI(uint32 const diff)
            {
                if (despawn)
                    return;

                float scale = me->GetFloatValue(OBJECT_FIELD_SCALE);
                if (scale <= 1.0f)
                    Despawn();

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_DARK_OF_NIGHT:
                    {
                        // Spawning 5 Dark of night
                        for (uint8 i = 0; i < 5; ++i)
                        {
                            Position spawnPos;
                            me->GetNearPosition(spawnPos, urand(30.0f, 50.0f), urand(0, 2 * M_PI));
                            if (Creature* dark = me->SummonCreature(NPC_DARK_OF_NIGHT, spawnPos))
                                darkList.push_back(dark->GetGUID());
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_sunbeamAI(creature);
        }
};

// 63446 - The Dark of Night
class npc_dark_of_night : public CreatureScript
{
    public:
        npc_dark_of_night() : CreatureScript("npc_dark_of_night") { }

        struct npc_dark_of_nightAI : public CreatureAI
        {
            InstanceScript* pInstance;
            uint64 sunbeamTargetGUID;
            uint64 m_SummonerGuid;
            uint32 visualCastTimer;
            EventMap m_Events;
            bool explode;

            npc_dark_of_nightAI(Creature* creature) : CreatureAI(creature)
            {
                pInstance = creature->GetInstanceScript();
                creature->SetReactState(REACT_PASSIVE);
                creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                creature->CastSpell(me, SPELL_VISUAL_DARK_OF_NIGHT, false);
                me->SetSpeed(MOVE_RUN, 0.5f);
                me->SetSpeed(MOVE_WALK, 0.5f);
                sunbeamTargetGUID = 0;
                visualCastTimer = 1000;
                explode = false;
            }

            void IsSummonedBy(Unit* summoner)
            {
                if (summoner)
                {
                    sunbeamTargetGUID = summoner->GetGUID();
                    DoCast(summoner, SPELL_VISUAL_LINK_TO_BEAM);
                    m_SummonerGuid = summoner->GetGUID();
                    m_Events.ScheduleEvent(EVENT_DARK_OF_NIGHT_MOVE, 500);
                }
            }

            void UpdateAI(uint32 const diff)
            {
                if (visualCastTimer <= diff)
                {
                    me->CastSpell(me, SPELL_VISUAL_DARK_OF_NIGHT, false);
                    visualCastTimer = 1000;
                }
                else
                    visualCastTimer -= diff;

                // Check if we are close enought to kill the sunbeam !
                if (!explode)
                {
                    Creature* sunbeam = me->GetMap()->GetCreature(sunbeamTargetGUID);
                    if (!sunbeam)
                    {
                        me->DespawnOrUnsummon();
                        return;
                    }

                    if (sunbeam->GetDistance(me) <= sunbeam->GetFloatValue(OBJECT_FIELD_SCALE))
                    {
                        me->CastSpell(me, SPELL_BUMP_DARK_OF_NIGHT, false);
                        me->Kill(sunbeam);
                        me->DespawnOrUnsummon(1000);
                        explode = true;
                    }
                }

                m_Events.Update(diff);

                if (m_Events.ExecuteEvent() == EVENT_DARK_OF_NIGHT_MOVE)
                {
                    if (Creature* summoner = Creature::GetCreature(*me, m_SummonerGuid))
                        me->GetMotionMaster()->MovePoint(1, summoner->GetPositionX(), summoner->GetPositionY(), summoner->GetPositionZ());
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_dark_of_nightAI(creature);
        }
};

// 63337 - Light of the day
class npc_ligth_of_day : public CreatureScript
{
    public:
        npc_ligth_of_day() : CreatureScript("npc_light_of_day") { }

        struct npc_light_of_dayAI : public ScriptedAI
        {
            npc_light_of_dayAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }

            void OnSpellClick(Unit* clicker)
            {
                if (clicker->GetTypeId() != TYPEID_PLAYER || clicker->HasAura(SPELL_SUN_BREATH))
                    return;
                else
                    me->DespawnOrUnsummon();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_light_of_dayAI(creature);
        }
};

// 62979 - Embodied Terror
class npc_embodied_terror : public CreatureScript
{
    public:
        npc_embodied_terror() : CreatureScript("npc_embodied_terror") { }

        struct npc_embodied_terrorAI : public CreatureAI
        {
            InstanceScript* pInstance;
            uint32 terrorizeTimer;
            EventMap m_Events;

            npc_embodied_terrorAI(Creature* creature) : CreatureAI(creature)
            {
                pInstance = creature->GetInstanceScript();
                terrorizeTimer = TIMER_TERRORIZE;
            }

            void IsSummonedBy(Unit* /*p_Summoner*/)
            {
                m_Events.ScheduleEvent(EVENT_EMBODIED_TERROR_MOVE, 500);
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                for (int i = 0; i < 5; i++)
                    me->CastSpell(me, SPELL_SUMMON_TINY_TERROR, false);
            }

            void UpdateAI(uint32 const diff)
            {
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(diff);

                if (Unit* victim = me->getVictim())
                {
                    if (victim->GetDisplayId() == DISPLAY_TSULON_NIGHT)
                        me->AttackStop();
                }

                if (terrorizeTimer <= diff)
                {
                    me->CastSpell(me, SPELL_TERRORIZE_PLAYER, false);
                    if (pInstance)
                    {
                        if (Creature* tsulong = pInstance->instance->GetCreature(pInstance->GetData64(NPC_TSULONG)))
                        {
                            if (tsulong->GetDisplayId() == DISPLAY_TSULON_DAY)
                                me->AddAura(SPELL_TERRORIZE_TSULONG, tsulong);
                        }
                    }
                    terrorizeTimer = TIMER_TERRORIZE;
                }
                else
                    terrorizeTimer -= diff;

                if (m_Events.ExecuteEvent() == EVENT_EMBODIED_TERROR_MOVE)
                    me->GetMotionMaster()->MovePoint(1, -1010.239f, -3043.97f, 12.82f);

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_embodied_terrorAI(creature);
        }
};

// 62977 - Fright Spawn
class npc_tiny_terror : public CreatureScript
{
    public:
        npc_tiny_terror() : CreatureScript("npc_tiny_terror") { }

        struct npc_tiny_terrorAI : public CreatureAI
        {
            InstanceScript* pInstance;
            uint32 frightTimer;

            npc_tiny_terrorAI(Creature* creature) : CreatureAI(creature)
            {
                pInstance = creature->GetInstanceScript();
                frightTimer = TIMER_FRIGHT;
            }

            void UpdateAI(uint32 const diff)
            {
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (Unit* victim = me->getVictim())
                {
                    if (victim->GetDisplayId() == DISPLAY_TSULON_NIGHT)
                        me->AttackStop();
                }

                if (frightTimer <= diff)
                {
                    me->CastSpell(me, SPELL_FRIGHT, false);
                    frightTimer = TIMER_TERRORIZE;
                }
                else
                    frightTimer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_tiny_terrorAI(creature);
        }
};

// 62919 - Unstable Sha
class npc_unstable_sha : public CreatureScript
{
    public:
        npc_unstable_sha() : CreatureScript("npc_unstable_sha") { }

        struct npc_unstable_shaAI : public CreatureAI
        {
            InstanceScript* pInstance;
            uint32 boltTimer;
            EventMap m_Events;

            npc_unstable_shaAI(Creature* creature) : CreatureAI(creature)
            {
                pInstance = creature->GetInstanceScript();
                boltTimer = TIMER_BOLT;
            }

            void Reset()
            {
                m_Events.Reset();
                me->SetSpeed(MOVE_RUN, 0.5f, true);
                me->SetSpeed(MOVE_WALK, 0.5f, true);
                me->SetReactState(REACT_PASSIVE);
                me->DisableHealthRegen();

                if (!pInstance)
                    return;

                m_Events.ScheduleEvent(EVENT_UNSTABLE_SHA_MOVE, 500);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!me->isAlive())
                    return;

                if (!pInstance)
                    return;

                if (Creature* tsulong = pInstance->instance->GetCreature(pInstance->GetData64(NPC_TSULONG)))
                {
                    if (tsulong->GetDistance(me) < 1.0f)
                    {
                        me->CastSpell(tsulong, SPELL_INSTABILITY, false);
                        me->Kill(me);
                    }
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (boltTimer <= diff)
                {
                    std::vector<Player*> plrRands;
                    Map::PlayerList const& players = me->GetMap()->GetPlayers();
                    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                    {
                        Player* plr = itr->getSource();
                        if (!plr)
                            continue;

                        if (plr->GetDistance(me) < 100.0f)
                            plrRands.push_back(plr);
                    }

                    if (plrRands.size())
                    {
                        me->CastSpell(plrRands[0], SPELL_BOLT, false);
                        me->DealDamage(me, 0.15 * me->GetMaxHealth());
                    }
                    boltTimer = TIMER_BOLT;
                }
                else
                    boltTimer -= diff;

                m_Events.Update(diff);

                if (m_Events.ExecuteEvent() == EVENT_UNSTABLE_SHA_MOVE)
                {
                    if (Creature* tsulong = pInstance->instance->GetCreature(pInstance->GetData64(NPC_TSULONG)))
                        me->GetMotionMaster()->MovePoint(1, tsulong->GetPositionX(), tsulong->GetPositionY(), tsulong->GetPositionZ());
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_unstable_shaAI(creature);
        }
};

// 125843, jam spell ? - Dread Shadows
class spell_dread_shadows_damage : public SpellScriptLoader
{
    public:
        spell_dread_shadows_damage() : SpellScriptLoader("spell_dread_shadows_damage") { }

        class spell_dread_shadows_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dread_shadows_damage_SpellScript);

            void RemoveInvalidTargets(std::list<WorldObject*>& targets)
            {
                targets.clear();
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dread_shadows_damage_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dread_shadows_damage_SpellScript::RemoveInvalidTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dread_shadows_damage_SpellScript();
        }
};

class DreadShadowsTargetCheck
{
    public:
        bool operator()(WorldObject* object) const
        {
            // check Sunbeam protection
            if (object->ToUnit() && object->ToUnit()->HasAura(122789))
                return true;

            return false;
        }
};

// 122768 - Dread Shadows
class spell_dread_shadows_malus : public SpellScriptLoader
{
    public:
        spell_dread_shadows_malus() : SpellScriptLoader("spell_dread_shadows_malus") { }

        class spell_dread_shadows_malus_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dread_shadows_malus_SpellScript);

            void RemoveInvalidTargets(std::list<WorldObject*>& targets)
            {
                targets.remove(GetCaster());
                targets.remove_if(DreadShadowsTargetCheck());
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dread_shadows_malus_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dread_shadows_malus_SpellScript::RemoveInvalidTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dread_shadows_malus_SpellScript();
        }
};

// 122789 - Sunbeam
class spell_sunbeam : public SpellScriptLoader
{
    public:
        spell_sunbeam() : SpellScriptLoader("spell_sunbeam") { }

        class spell_sunbeam_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sunbeam_SpellScript);

            void CheckTargets(std::list<WorldObject*>& targets)
            {
                targets.clear();
                Map::PlayerList const& players = GetCaster()->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                {
                    Player* plr = itr->getSource();
                    if (!plr)
                        continue;

                    float scale = GetCaster()->GetFloatValue(OBJECT_FIELD_SCALE);
                    if (plr->GetExactDist2d(GetCaster()) <= scale)
                        targets.push_back(plr);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sunbeam_SpellScript::CheckTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sunbeam_SpellScript();
        }


        class spell_sunbeam_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sunbeam_aura_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetTarget()->IsPlayer())
                {
                    if (Unit* l_Caster = GetCaster())
                    {
                        if (Pet* pet = GetTarget()->ToPlayer()->GetPet())
                            pet->AddAura(SPELL_SUNBEAM_PROTECTION, pet);

                        float scale = l_Caster->GetFloatValue(OBJECT_FIELD_SCALE);
                        if (scale > 0.2f)
                            l_Caster->SetObjectScale(scale - 0.2f);
                    }
                }

                GetTarget()->RemoveAurasDueToSpell(SPELL_DREAD_SHADOWS_DEBUFF);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetTarget()->IsPlayer())
                {
                    if (Pet* pet = GetTarget()->ToPlayer()->GetPet())
                        pet->RemoveAurasDueToSpell(SPELL_SUNBEAM_PROTECTION);
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_sunbeam_aura_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_sunbeam_aura_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sunbeam_aura_AuraScript();
        }
};

// 122855 - Sun Breath
class spell_sun_breath : public SpellScriptLoader
{
    public:
        spell_sun_breath() : SpellScriptLoader("spell_sun_breath") { }

        class spell_sun_breath_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sun_breath_SpellScript);

            void HandleAfterCast()
            {
                Unit* caster = GetCaster();
                if (!caster)
                    return;

                // spell still have valid caster !
                Map::PlayerList const& players = caster->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                {
                    Player* plr = itr->getSource();
                    if (plr->GetDistance(caster) < 30.0f && plr->isInFront(caster, M_PI/3) && !plr->HasAura(SPELL_BUFF_LIGHT_OF_DAY))
                        caster->AddAura(SPELL_BATHED_IN_LIGHT, itr->getSource());
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_sun_breath_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sun_breath_SpellScript();
        }
};

// 123018 - Terrorize
class spell_terrorize_player : public SpellScriptLoader
{
    public:
        spell_terrorize_player() : SpellScriptLoader("spell_terrorize_player") { }

        class spell_terrorize_player_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_terrorize_player_SpellScript);

            void DealDamage()
            {
                Unit* target = GetHitUnit();
                if (!target)
                    return;

                SetHitDamage(std::max(GetSpellInfo()->Effects[0].BasePoints * 1000, int((target->GetHealth()*GetSpellInfo()->Effects[0].BasePoints)/100)));
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_terrorize_player_SpellScript::DealDamage);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_terrorize_player_SpellScript();
        }
};
// 123697 - Instability
class spell_instability : public SpellScriptLoader
{
    public:
        spell_instability() : SpellScriptLoader("spell_instability") { }

        class spell_instability_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_instability_SpellScript);

            void CheckTargets(WorldObject*& target)
            {
                target = NULL;
                InstanceScript* pInstance = GetCaster()->GetInstanceScript();
                if (!pInstance)
                    return;

                if (Creature* tsulong = GetCaster()->GetMap()->GetCreature(pInstance->GetData64(NPC_TSULONG)))
                   target = tsulong;
            }

            void Register()
            {
                OnObjectTargetSelect += SpellObjectTargetSelectFn(spell_instability_SpellScript::CheckTargets, EFFECT_0, TARGET_UNIT_NEARBY_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_instability_SpellScript();
        }
};

// 123716 - Light of the day
class spell_light_of_the_day : public SpellScriptLoader
{
    public:
        spell_light_of_the_day() : SpellScriptLoader("spell_light_of_the_day") { }

        class spell_light_of_the_day_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_light_of_the_day_AuraScript);

            void Check(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = GetTarget())
                    if (target->HasAura(SPELL_SUN_BREATH))
                        target->RemoveAura(SPELL_BUFF_LIGHT_OF_DAY);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_light_of_the_day_AuraScript::Check, EFFECT_1, SPELL_AURA_MOD_HEALING_DONE_PERCENT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_light_of_the_day_AuraScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_tsulong()
{
    new boss_tsulong();                 // 62442
    new npc_sunbeam();                  // 62849
    new npc_dark_of_night();            // 63346
    new npc_ligth_of_day();             // 63337
    new npc_embodied_terror();          // 62969
    new npc_tiny_terror();              // 62977
    new npc_unstable_sha();             // 62919
    new spell_dread_shadows_damage();   // 125843
    new spell_dread_shadows_malus();    // 122768
    new spell_sunbeam();                // 122789
    new spell_sun_breath();             // 122855
    new spell_terrorize_player();       // 123018
    new spell_instability();            // 123697
    new spell_light_of_the_day();       // 123716
}
#endif
