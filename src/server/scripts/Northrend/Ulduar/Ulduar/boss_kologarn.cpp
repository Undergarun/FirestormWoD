/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "ulduar.h"
#include "Vehicle.h"

enum Spells
{
    SPELL_ARM_DEAD_DAMAGE_10            = 63629,    // not used
    SPELL_ARM_DEAD_DAMAGE_25            = 63979,    // not used
    SPELL_TWO_ARM_SMASH_10              = 63356,
    SPELL_TWO_ARM_SMASH_25              = 64003,
    SPELL_ONE_ARM_SMASH_10              = 63573,
    SPELL_ONE_ARM_SMASH_25              = 64006,
    SPELL_ARM_SWEEP_10                  = 63766,
    SPELL_ARM_SWEEP_25                  = 63983,
    SPELL_STONE_SHOUT_10                = 63716,
    SPELL_STONE_SHOUT_25                = 64005,
    SPELL_PETRIFY_BREATH_10             = 62030,
    SPELL_PETRIFY_BREATH_25             = 63980,
    SPELL_STONE_GRIP_10                 = 62166,
    SPELL_STONE_GRIP_25                 = 63981,
    SPELL_STONE_GRIP_CANCEL             = 65594,
    SPELL_STONE_GRIP_DOT_10             = 64290,
    SPELL_STONE_GRIP_DOT_25             = 64292,
    SPELL_SUMMON_RUBBLE                 = 63633,
    SPELL_FALLING_RUBBLE                = 63821,
    SPELL_ARM_ENTER_VEHICLE             = 65343,
    SPELL_ARM_ENTER_VISUAL              = 64753,
    SPELL_SUMMON_FOCUSED_EYEBEAM        = 63342,
    SPELL_FOCUSED_EYEBEAM_PERIODIC_10   = 63347,
    SPELL_FOCUSED_EYEBEAM_PERIODIC_25   = 63977,
    SPELL_FOCUSED_EYEBEAM_VISUAL        = 63369,
    SPELL_FOCUSED_EYEBEAM_VISUAL_LEFT   = 63676,
    SPELL_FOCUSED_EYEBEAM_VISUAL_RIGHT  = 63702,
    SPELL_FOCUSED_EYEBEAM_DAMAGE_10     = 63346,
    SPELL_FOCUSED_EYEBEAM_DAMAGE_25     = 63976,
    SPELL_KOLOGARN_REDUCE_PARRY         = 64651,
    SPELL_KOLOGARN_PACIFY               = 63726,
    SPELL_BERSERK                       = 47008,
};

#define SPELL_ARM_DEAD_DAMAGE           RAID_MODE(SPELL_ARM_DEAD_DAMAGE_10, SPELL_ARM_DEAD_DAMAGE_25)
#define SPELL_TWO_ARM_SMASH             RAID_MODE(SPELL_TWO_ARM_SMASH_10, SPELL_TWO_ARM_SMASH_25)
#define SPELL_ONE_ARM_SMASH             RAID_MODE(SPELL_ONE_ARM_SMASH_10, SPELL_ONE_ARM_SMASH_25)
#define SPELL_ARM_SWEEP                 RAID_MODE(SPELL_ARM_SWEEP_10, SPELL_ARM_SWEEP_25)
#define SPELL_STONE_SHOUT               RAID_MODE(SPELL_STONE_SHOUT_10, SPELL_STONE_SHOUT_25)
#define SPELL_PETRIFY_BREATH            RAID_MODE(SPELL_PETRIFY_BREATH_10, SPELL_PETRIFY_BREATH_25)
#define SPELL_STONE_GRIP                RAID_MODE(SPELL_STONE_GRIP_10, SPELL_STONE_GRIP_25)
#define SPELL_FOCUSED_EYEBEAM_PERIODIC  RAID_MODE(SPELL_FOCUSED_EYEBEAM_PERIODIC_10, SPELL_FOCUSED_EYEBEAM_PERIODIC_25)
#define SPELL_STONE_GRIP_DOT            RAID_MODE(SPELL_STONE_GRIP_DOT_10, SPELL_STONE_GRIP_DOT_25)
#define SPELL_FOCUSED_EYEBEAM_DAMAGE    RAID_MODE(SPELL_FOCUSED_EYEBEAM_DAMAGE_10, SPELL_FOCUSED_EYEBEAM_DAMAGE_25)

enum Events
{
    EVENT_INSTALL_ACCESSORIES   = 1,
    EVENT_MELEE_CHECK           = 2,
    EVENT_SMASH                 = 3,
    EVENT_SWEEP                 = 4,
    EVENT_STONE_SHOUT           = 5,
    EVENT_STONE_GRIP            = 6,
    EVENT_FOCUSED_EYEBEAM       = 7,
    EVENT_RESPAWN_LEFT_ARM      = 8,
    EVENT_RESPAWN_RIGHT_ARM     = 9,
    EVENT_ENRAGE                = 10,
};

enum Yells
{
    SAY_AGGRO                   = 0,
    SAY_SLAY                    = 1,
    SAY_LEFT_ARM_GONE           = 2,
    SAY_RIGHT_ARM_GONE          = 3,
    SAY_SHOCKWAVE               = 4,
    SAY_GRAB_PLAYER             = 5,
    SAY_DEATH                   = 6,
    SAY_BERSERK                 = 7,
    EMOTE_LEFT                  = 8,
    EMOTE_RIGHT                 = 9,
    EMOTE_STONE                 = 10,
};

enum DataTypes
{
    DATA_DISARMED_EVENT         = 21687,

    DATA_RUBBLE_AND_ROLL        = 1,
    DATA_DISARMED               = 2,
    DATA_WITH_OPEN_ARMS         = 3,
    DATA_IF_LOOKS_COULD_KILL    = 4
};

class boss_kologarn : public CreatureScript
{
    public:
        boss_kologarn() : CreatureScript("boss_kologarn") { }

        struct boss_kologarnAI : public BossAI
        {
            boss_kologarnAI(Creature* creature) : BossAI(creature, BOSS_KOLOGARN), vehicle(creature->GetVehicleKit()),
                haveLeftArm(false), haveRightArm(false)
            {
                ASSERT(vehicle);

                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                me->SetDisableGravity(true);
                me->SetStandState(UNIT_STAND_STATE_SUBMERGED);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
                emerged = false;

                DoCast(SPELL_KOLOGARN_REDUCE_PARRY);
                SetCombatMovement(false);
                Reset();
            }

            void EnterCombat(Unit* who)
            {
                if (!instance->CheckRequiredBosses(BOSS_KOLOGARN, who->ToPlayer()))
                {
                    EnterEvadeMode();
                    instance->DoCastSpellOnPlayers(SPELL_BASE_CAMP_TELEPORT);
                    return;
                }

                Talk(SAY_AGGRO);

                events.ScheduleEvent(EVENT_MELEE_CHECK, 6000);
                events.ScheduleEvent(EVENT_SMASH, 5000);
                events.ScheduleEvent(EVENT_SWEEP, 19000);
                events.ScheduleEvent(EVENT_STONE_GRIP, 25000);
                events.ScheduleEvent(EVENT_FOCUSED_EYEBEAM, 21000);
                events.ScheduleEvent(EVENT_ENRAGE, 600000);

                _EnterCombat();
            }

            void Reset()
            {
                _Reset();
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                eyebeamTarget = 0;
                _rubbleCount = 0;
                _armDied = false;
                _ifLooksCouldKill = true;
                summons.DespawnAll();

                instance->DoStopTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, DATA_DISARMED_EVENT);
            }

            void MoveInLineOfSight(Unit* who)
            {
                // Birth animation
                if (!emerged && me->IsWithinDistInMap(who, 40.0f) && who->ToPlayer() && !who->ToPlayer()->isGameMaster())
                {
                    me->SetStandState(UNIT_STAND_STATE_STAND);
                    me->HandleEmoteCommand(EMOTE_ONESHOT_EMERGE);
                    emerged = true;
                }
                BossAI::MoveInLineOfSight(who);
            }

            void JustDied(Unit* /*victim*/)
            {
                for (uint8 i = 0; i < 2; ++i)
                    if (Unit* arm = vehicle->GetPassenger(i))
                        arm->ExitVehicle();

                Talk(SAY_DEATH);
                DoCast(me, SPELL_KOLOGARN_PACIFY);
                me->GetMotionMaster()->MoveTargetedHome();
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                me->SetCorpseDelay(604800); // Prevent corpse from despawning.
                summons.DespawnAll();
                _JustDied();
            }

            void KilledUnit(Unit* who)
            {
                if (!urand(0,5))
                    Talk(SAY_SLAY);

                if (who->GetTypeId() == TYPEID_PLAYER)
                    who->ExitVehicle();
            }

            void PassengerBoarded(Unit* who, int8 /*seatId*/, bool apply)
            {
                bool isEncounterInProgress = instance->GetBossState(BOSS_KOLOGARN) == IN_PROGRESS;
                if (who->GetEntry() == NPC_LEFT_ARM)
                {
                    haveLeftArm = apply;
                    if (!apply && isEncounterInProgress)
                    {
                        who->ToCreature()->DisappearAndDie();
                        Talk(SAY_LEFT_ARM_GONE);
                        events.ScheduleEvent(EVENT_RESPAWN_LEFT_ARM, 40000);
                    }
                }

                else if (who->GetEntry() == NPC_RIGHT_ARM)
                {
                    haveRightArm = apply;
                    if (!apply && isEncounterInProgress)
                    {
                        who->ToCreature()->DisappearAndDie();
                        Talk(SAY_RIGHT_ARM_GONE);
                        events.ScheduleEvent(EVENT_RESPAWN_RIGHT_ARM, 40000);
                    }
                }

                if (!isEncounterInProgress)
                    return;

                if (!apply)
                {
                    _armDied = true;
                    int32 damagedHealth = Is25ManRaid() ? 2300925 : 543855;
                    uint32 currHealth = me->GetHealth();

                    if (me && me->getVictim() && damagedHealth >= int32(currHealth))
                        me->getVictim()->Kill(me);
                    else
                    {
                        me->ModifyHealth(-(damagedHealth));
                        me->LowerPlayerDamageReq(damagedHealth);
                    }

                    Creature* rubbleStalker = NULL;

                    std::list<Creature*> rubbeStalkers;
                    me->GetCreatureListWithEntryInGrid(rubbeStalkers, NPC_RUBBLE_STALKER, 70.0f);
                    if (!rubbeStalkers.empty())
                    {
                        for (std::list<Creature*>::iterator itr = rubbeStalkers.begin(); itr != rubbeStalkers.end(); ++itr)
                        {
                            if (who->GetEntry() == NPC_RIGHT_ARM && (*itr)->GetPositionY() > -3.6f && (*itr)->GetPositionY() < -2.0f)
                            {
                                rubbleStalker = (*itr);
                                break;
                            }

                            if (who->GetEntry() == NPC_LEFT_ARM && (*itr)->GetPositionY() > -46.6f && (*itr)->GetPositionY() < -43.0f)
                            {
                                rubbleStalker = (*itr);
                                break;
                            }
                        }
                    }

                    if (rubbleStalker)
                    {
                        rubbleStalker->CastSpell(rubbleStalker, SPELL_FALLING_RUBBLE, true);
                        rubbleStalker->CastSpell(rubbleStalker, SPELL_SUMMON_RUBBLE, true);
                    }

                    if (!haveRightArm && !haveLeftArm)
                        events.ScheduleEvent(EVENT_STONE_SHOUT, 5000);

                    instance->DoStartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, DATA_DISARMED_EVENT);
                }
                else
                {
                    instance->DoStopTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, DATA_DISARMED_EVENT);
                    events.CancelEvent(EVENT_STONE_SHOUT);
                    who->ToCreature()->SetInCombatWithZone();
                }
            }

            void JustSummoned(Creature* summon)
            {
                switch (summon->GetEntry())
                {
                    case NPC_FOCUSED_EYEBEAM:
                        summon->CastSpell(me, SPELL_FOCUSED_EYEBEAM_VISUAL_LEFT, true);
                        break;
                    case NPC_FOCUSED_EYEBEAM_RIGHT:
                        summon->CastSpell(me, SPELL_FOCUSED_EYEBEAM_VISUAL_RIGHT, true);
                        break;
                    case NPC_RUBBLE:
                        summons.Summon(summon);
                        summon->SetInCombatWithZone();
                        ++_rubbleCount;
                        return;
                    default:
                        return;
                }

                summon->CastSpell(summon, SPELL_FOCUSED_EYEBEAM_PERIODIC, true);
                summon->CastSpell(summon, SPELL_FOCUSED_EYEBEAM_VISUAL, true);
                summon->SetReactState(REACT_PASSIVE);
                // One of the above spells is a channeled spell, we need to clear this unit state for MoveChase to work
                summon->ClearUnitState(UNIT_STATE_CASTING);

                // Victim gets 67351
                if (eyebeamTarget)
                {
                    if (Unit* target = Unit::GetUnit(*summon, eyebeamTarget))
                    {
                        summon->Attack(target, false);
                        summon->GetMotionMaster()->MoveChase(target);
                    }
                }
            }

            void SetData(uint32 type, uint32 data)
            {
                switch (type)
                {
                    case DATA_IF_LOOKS_COULD_KILL:
                        _ifLooksCouldKill = data;
                        break;
                    default:
                        break;
                }
            }

            uint32 GetData(uint32 type)
            {
                switch (type)
                {
                    case DATA_RUBBLE_AND_ROLL:
                        return (_rubbleCount >= 25) ? 1 : 0;
                    case DATA_DISARMED:
                        return (!haveRightArm && !haveLeftArm) ? 1 : 0;
                    case DATA_WITH_OPEN_ARMS:
                        return _armDied ? 0 : 1;
                    case DATA_IF_LOOKS_COULD_KILL:
                        return _ifLooksCouldKill ? 1 : 0;
                    default:
                        break;
                }

                return 0;
            }

            // try to get ranged target, not too far away
            Player* GetEyeBeamTarget()
            {
                Map* map = me->GetMap();
                if (map && map->IsDungeon())
                {
                    std::list<Player*> playerList;
                    Map::PlayerList const& players = map->GetPlayers();
                    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                    {
                        if (Player* player = itr->getSource())
                        {
                            if (player->isDead() || player->HasAura(SPELL_STONE_GRIP_DOT) || player->isGameMaster())
                                continue;

                            if (me->getVictim())
                                if (me->getVictim()->GetGUID() == player->GetGUID())
                                    continue;

                            float Distance = player->GetDistance(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
                            if (Distance < 20.0f || Distance > 60.0f)
                                continue;

                            playerList.push_back(player);
                        }
                    }

                    if (playerList.empty()) // This should only happen if our current victim is the only alive one
                    {
                        Unit* sel = SelectTarget(SELECT_TARGET_RANDOM, 0, 60.0f, true, -SPELL_STONE_GRIP_DOT);
                        if (sel)
                            if (Player* p = sel->ToPlayer())
                                playerList.push_back(p);
                            else
                                return 0;
                        else
                            return 0;
                    }

                    return JadeCore::Containers::SelectRandomContainerElement(playerList);
                }
                else
                    return 0;
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_MELEE_CHECK:
                            if (!me->IsWithinMeleeRange(me->getVictim()))
                                DoCast(SPELL_PETRIFY_BREATH);
                            events.ScheduleEvent(EVENT_MELEE_CHECK, 1 * IN_MILLISECONDS);
                            break;
                        case EVENT_SWEEP:
                            if (haveLeftArm)
                            {
                                if (Creature* target = me->FindNearestCreature(NPC_ARM_SWEEP_STALKER, 500.0f, true))
                                {
                                    DoCast(target, SPELL_ARM_SWEEP, true);
                                    Talk(SAY_SHOCKWAVE);
                                }
                            }
                            events.ScheduleEvent(EVENT_SWEEP, 25 * IN_MILLISECONDS);
                            break;
                        case EVENT_SMASH:
                            if (haveLeftArm && haveRightArm)
                                DoCastVictim(SPELL_TWO_ARM_SMASH);
                            else if (haveLeftArm || haveRightArm)
                                DoCastVictim(SPELL_ONE_ARM_SMASH);
                            events.ScheduleEvent(EVENT_SMASH, 15 * IN_MILLISECONDS);
                            break;
                        case EVENT_STONE_SHOUT:
                            DoCast(SPELL_STONE_SHOUT);
                            events.ScheduleEvent(EVENT_STONE_SHOUT, 2 * IN_MILLISECONDS);
                            break;
                        case EVENT_ENRAGE:
                            DoCast(SPELL_BERSERK);
                            Talk(SAY_BERSERK);
                            break;
                        case EVENT_RESPAWN_LEFT_ARM:
                        case EVENT_RESPAWN_RIGHT_ARM:
                        {
                            if (vehicle)
                            {
                                int8 seat = eventId == EVENT_RESPAWN_LEFT_ARM ? 0 : 1;
                                uint32 entry = eventId == EVENT_RESPAWN_LEFT_ARM ? NPC_LEFT_ARM : NPC_RIGHT_ARM;
                                vehicle->InstallAccessory(entry, seat, true, TEMPSUMMON_MANUAL_DESPAWN, 0);
                            }
                            Talk(eventId == EVENT_RESPAWN_LEFT_ARM ? EMOTE_LEFT : EMOTE_RIGHT);
                            break;
                        }
                        case EVENT_STONE_GRIP:
                        {
                            if (haveRightArm)
                            {
                                DoCast(SPELL_STONE_GRIP);
                                Talk(SAY_GRAB_PLAYER);
                                Talk(EMOTE_STONE);
                            }
                            events.ScheduleEvent(EVENT_STONE_GRIP, 25 * IN_MILLISECONDS);
                        }
                        break;
                        case EVENT_FOCUSED_EYEBEAM:
                            if (Unit* eyebeamTargetUnit = GetEyeBeamTarget())
                            {
                                eyebeamTarget = eyebeamTargetUnit->GetGUID();
                                DoCast(me, SPELL_SUMMON_FOCUSED_EYEBEAM, true);
                            }
                            events.ScheduleEvent(EVENT_FOCUSED_EYEBEAM, urand(15, 35) * IN_MILLISECONDS);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        private:
            Vehicle* vehicle;
            bool haveLeftArm, haveRightArm;
            uint64 eyebeamTarget;
            uint8 _rubbleCount;
            bool _armDied;
            bool _ifLooksCouldKill;
            bool emerged;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetUlduarAI<boss_kologarnAI>(creature);
        }
};

class npc_kologarn_arm : public CreatureScript
{
    public:
        npc_kologarn_arm() : CreatureScript("npc_kologarn_arm") {}

        struct npc_kologarn_armAI : public ScriptedAI
        {
            npc_kologarn_armAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
            }

            void JustDied(Unit* /*victim*/)
            {
                if (me->GetEntry() == NPC_RIGHT_ARM)
                    DoCast(me, SPELL_STONE_GRIP_CANCEL);
                me->ExitVehicle();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_kologarn_armAI(creature);
        }
};

class npc_focused_eyebeam : public CreatureScript
{
    public:
        npc_focused_eyebeam() : CreatureScript("npc_focused_eyebeam") { }

        struct npc_focused_eyebeamAI : public ScriptedAI
        {
            npc_focused_eyebeamAI(Creature* creature) : ScriptedAI(creature)
            {
                _instance = me->GetInstanceScript();
            }

            void SpellHitTarget(Unit* target, SpellInfo const* spell)
            {
                if (spell->Id == SPELL_FOCUSED_EYEBEAM_DAMAGE_10 || spell->Id == SPELL_FOCUSED_EYEBEAM_DAMAGE_25)
                    if (Creature* kologarn = ObjectAccessor::GetCreature(*me, _instance->GetData64(BOSS_KOLOGARN)))
                        kologarn->AI()->SetData(DATA_IF_LOOKS_COULD_KILL, false);
            }

        private:
            InstanceScript* _instance;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_focused_eyebeamAI(creature);
        }
};

class spell_ulduar_rubble_summon : public SpellScriptLoader
{
    public:
        spell_ulduar_rubble_summon() : SpellScriptLoader("spell_ulduar_rubble_summon") { }

        class spell_ulduar_rubble_summonSpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ulduar_rubble_summonSpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (!caster)
                    return;

                uint64 originalCaster = caster->GetInstanceScript() ? caster->GetInstanceScript()->GetData64(BOSS_KOLOGARN) : 0;
                uint32 spellId = GetEffectValue();
                for (uint8 i = 0; i < 5; ++i)
                    caster->CastSpell(caster, spellId, true, NULL, NULL, originalCaster);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_ulduar_rubble_summonSpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_ulduar_rubble_summonSpellScript();
        }
};

// predicate function to select non main tank target
class StoneGripTargetSelector : public std::unary_function<Unit*, bool>
{
    public:
        StoneGripTargetSelector(Creature* me, Unit const* victim) : _me(me), _victim(victim) {}

        bool operator()(WorldObject* target)
        {
            if (target == _victim && _me->getThreatManager().getThreatList().size() > 1)
                return true;

            if (target->GetTypeId() != TYPEID_PLAYER)
                return true;

            return false;
        }

        Creature* _me;
        Unit const* _victim;
};

class spell_ulduar_stone_grip_cast_target : public SpellScriptLoader
{
    public:
        spell_ulduar_stone_grip_cast_target() : SpellScriptLoader("spell_ulduar_stone_grip_cast_target") { }

        class spell_ulduar_stone_grip_cast_target_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ulduar_stone_grip_cast_target_SpellScript);

            bool Load()
            {
                if (GetCaster()->GetTypeId() != TYPEID_UNIT)
                    return false;
                return true;
            }

            void FilterTargetsInitial(std::list<WorldObject*>& unitList)
            {
                // Remove "main tank" and non-player targets
                unitList.remove_if(StoneGripTargetSelector(GetCaster()->ToCreature(), GetCaster()->getVictim()));
                // Maximum affected targets per difficulty mode
                uint32 maxTargets = GetSpellInfo()->Id == 63981 ? 3 : 1;

                // Return a random amount of targets based on maxTargets
                while (maxTargets < unitList.size())
                {
                    std::list<WorldObject*>::iterator itr = unitList.begin();
                    advance(itr, urand(0, unitList.size()-1));
                    unitList.erase(itr);
                }

                // For subsequent effects
                m_unitList = unitList;
            }

            void FillTargetsSubsequential(std::list<WorldObject*>& unitList)
            {
                unitList = m_unitList;
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ulduar_stone_grip_cast_target_SpellScript::FilterTargetsInitial, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ulduar_stone_grip_cast_target_SpellScript::FillTargetsSubsequential, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ulduar_stone_grip_cast_target_SpellScript::FillTargetsSubsequential, EFFECT_2, TARGET_UNIT_SRC_AREA_ENEMY);
            }

            // Shared between effects
            std::list<WorldObject*> m_unitList;
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_ulduar_stone_grip_cast_target_SpellScript();
        }
};

class spell_ulduar_cancel_stone_grip : public SpellScriptLoader
{
    public:
        spell_ulduar_cancel_stone_grip() : SpellScriptLoader("spell_ulduar_cancel_stone_grip") { }

        class spell_ulduar_cancel_stone_gripSpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ulduar_cancel_stone_gripSpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Unit* target = GetHitUnit();
                if (!target || !target->GetVehicle())
                    return;

                switch (target->GetMap()->GetDifficulty())
                {
                    case MAN10_DIFFICULTY:
                        target->RemoveAura(GetSpellInfo()->Effects[EFFECT_0].CalcValue());
                        break;
                    case MAN25_DIFFICULTY:
                        target->RemoveAura(GetSpellInfo()->Effects[EFFECT_1].CalcValue());
                        break;
                    default:
                        break;
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_ulduar_cancel_stone_gripSpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_ulduar_cancel_stone_gripSpellScript();
        }
};

class spell_ulduar_squeezed_lifeless : public SpellScriptLoader
{
    public:
        spell_ulduar_squeezed_lifeless() : SpellScriptLoader("spell_ulduar_squeezed_lifeless") { }

        class spell_ulduar_squeezed_lifeless_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ulduar_squeezed_lifeless_SpellScript);

            void HandleInstaKill(SpellEffIndex /*effIndex*/)
            {
                if (!GetHitPlayer() || !GetHitPlayer()->GetVehicle())
                    return;

                //! Proper exit position does not work currently,
                //! See documentation in void Unit::ExitVehicle(Position const* exitPosition)
                Position pos;
                pos.m_positionX = 1756.25f + irand(-3, 3);
                pos.m_positionY = -8.3f + irand(-3, 3);
                pos.m_positionZ = 448.8f;
                pos.m_orientation = M_PI;
                GetHitPlayer()->DestroyForNearbyPlayers();
                GetHitPlayer()->ExitVehicle(&pos);
                GetHitPlayer()->UpdateObjectVisibility(false);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_ulduar_squeezed_lifeless_SpellScript::HandleInstaKill, EFFECT_1, SPELL_EFFECT_INSTAKILL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_ulduar_squeezed_lifeless_SpellScript();
        }
};

class spell_ulduar_stone_grip_absorb : public SpellScriptLoader
{
    public:
        spell_ulduar_stone_grip_absorb() : SpellScriptLoader("spell_ulduar_stone_grip_absorb") { }

        class spell_ulduar_stone_grip_absorb_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ulduar_stone_grip_absorb_AuraScript);

            //! This will be called when Right Arm (vehicle) has sustained a specific amount of damage depending on instance mode
            //! What we do here is remove all harmful aura's related and teleport to safe spot.
            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetTargetApplication()->GetRemoveMode() !=  AURA_REMOVE_BY_ENEMY_SPELL)
                    return;

                if (!GetOwner()->ToCreature())
                    return;

                if (Vehicle * vehicle = GetOwner()->ToCreature()->GetVehicle())
                {
                    if (Unit* rightArm = vehicle->GetPassenger(1))
                    {
                        if (Unit* pass = rightArm->GetVehicleKit()->GetPassenger(0))
                        {
                            Position exitPosition;
                            exitPosition.m_positionX = 1750.0f;
                            exitPosition.m_positionY = -7.5f + frand(-3.0f, 3.0f);
                            exitPosition.m_positionZ = 457.9322f;

                            pass->_ExitVehicle(&exitPosition);
                            pass->RemoveAurasDueToSpell(62056);

                            Position oldPos;
                            pass->GetPosition(&oldPos);
                            pass->Relocate(exitPosition);
                            pass->GetMotionMaster()->MoveFall();
                            pass->Relocate(oldPos);
                            rightArm->GetVehicleKit()->Reset();
                        }
                    }
                }
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_ulduar_stone_grip_absorb_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_ulduar_stone_grip_absorb_AuraScript();
        }
};

class spell_ulduar_stone_grip : public SpellScriptLoader
{
    public:
        spell_ulduar_stone_grip() : SpellScriptLoader("spell_ulduar_stone_grip") { }

        class spell_ulduar_stone_grip_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ulduar_stone_grip_AuraScript);

            void OnRemoveStun(constAuraEffectPtr aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Player* owner = GetOwner()->ToPlayer())
                    owner->RemoveAurasDueToSpell(aurEff->GetAmount());
            }

            void OnEnterVehicle(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Player* caster = GetCaster() ? GetCaster()->ToPlayer() : 0;
                if (caster)
                    caster->ClearUnitState(UNIT_STATE_ONVEHICLE);
            }

            void OnRemoveVehicle(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes mode)
            {
                PreventDefaultAction();
                Unit* caster = GetCaster();
                if (!caster)
                    return;

                Position exitPosition;
                exitPosition.m_positionX = 1750.0f;
                exitPosition.m_positionY = -7.5f + frand(-3.0f, 3.0f);
                exitPosition.m_positionZ = 457.9322f;

                caster->_ExitVehicle(&exitPosition);
                caster->RemoveAurasDueToSpell(GetId());

                Position oldPos;
                caster->GetPosition(&oldPos);
                caster->Relocate(exitPosition);
                caster->GetMotionMaster()->MoveFall();
                caster->Relocate(oldPos);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_ulduar_stone_grip_AuraScript::OnEnterVehicle, EFFECT_0, SPELL_AURA_CONTROL_VEHICLE, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_ulduar_stone_grip_AuraScript::OnRemoveVehicle, EFFECT_0, SPELL_AURA_CONTROL_VEHICLE, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_ulduar_stone_grip_AuraScript::OnRemoveStun, EFFECT_2, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_ulduar_stone_grip_AuraScript();
        }
};

class spell_kologarn_stone_shout : public SpellScriptLoader
{
    public:
        spell_kologarn_stone_shout() : SpellScriptLoader("spell_kologarn_stone_shout") { }

        class spell_kologarn_stone_shout_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_kologarn_stone_shout_SpellScript);

            void FilterTargets(std::list<WorldObject*>& unitList)
            {
                unitList.remove_if(PlayerOrPetCheck());
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_kologarn_stone_shout_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_kologarn_stone_shout_SpellScript();
        }
};

class spell_kologarn_summon_focused_eyebeam : public SpellScriptLoader
{
    public:
        spell_kologarn_summon_focused_eyebeam() : SpellScriptLoader("spell_kologarn_summon_focused_eyebeam") { }

        class spell_kologarn_summon_focused_eyebeam_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_kologarn_summon_focused_eyebeam_SpellScript);

            void HandleForceCast(SpellEffIndex eff)
            {
                PreventHitDefaultEffect(eff);
                GetCaster()->CastSpell(GetCaster(), GetSpellInfo()->Effects[eff].TriggerSpell, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_kologarn_summon_focused_eyebeam_SpellScript::HandleForceCast, EFFECT_0, SPELL_EFFECT_FORCE_CAST);
                OnEffectHitTarget += SpellEffectFn(spell_kologarn_summon_focused_eyebeam_SpellScript::HandleForceCast, EFFECT_1, SPELL_EFFECT_FORCE_CAST);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_kologarn_summon_focused_eyebeam_SpellScript();
        }
};

class achievement_rubble_and_roll : public AchievementCriteriaScript
{
    public:
        achievement_rubble_and_roll(const char* name) : AchievementCriteriaScript(name) { }

        bool OnCheck(Player* /*source*/, Unit* target)
        {
            if (target && target->IsAIEnabled)
                return target->GetAI()->GetData(DATA_RUBBLE_AND_ROLL);

            return false;
        }
};

class achievement_with_open_arms : public AchievementCriteriaScript
{
    public:
        achievement_with_open_arms(const char* name) : AchievementCriteriaScript(name) { }

        bool OnCheck(Player* /*source*/, Unit* target)
        {
            if (target && target->IsAIEnabled)
                return target->GetAI()->GetData(DATA_WITH_OPEN_ARMS);

            return false;
        }
};

class achievement_if_looks_could_kill : public AchievementCriteriaScript
{
    public:
        achievement_if_looks_could_kill(const char* name) : AchievementCriteriaScript(name) { }

        bool OnCheck(Player* /*source*/, Unit* target)
        {
            if (target && target->IsAIEnabled)
                return target->GetAI()->GetData(DATA_IF_LOOKS_COULD_KILL);

            return false;
        }
};

class achievement_disarmed : public AchievementCriteriaScript
{
    public:
        achievement_disarmed(const char* name) : AchievementCriteriaScript(name) { }

        bool OnCheck(Player* /*source*/, Unit* target)
        {
            if (target && target->IsAIEnabled)
                return target->GetAI()->GetData(DATA_DISARMED);

            return false;
        }
};

void AddSC_boss_kologarn()
{
    new boss_kologarn();
    new npc_kologarn_arm();
    new npc_focused_eyebeam();
    new spell_ulduar_rubble_summon();
    new spell_ulduar_squeezed_lifeless();
    new spell_ulduar_cancel_stone_grip();
    new spell_ulduar_stone_grip_cast_target();
    new spell_ulduar_stone_grip_absorb();
    new spell_ulduar_stone_grip();
    new spell_kologarn_stone_shout();
    new spell_kologarn_summon_focused_eyebeam();

    new achievement_rubble_and_roll("achievement_rubble_and_roll");
    new achievement_rubble_and_roll("achievement_rubble_and_roll_25");
    new achievement_if_looks_could_kill("achievement_if_looks_could_kill");
    new achievement_if_looks_could_kill("achievement_if_looks_could_kill_25");
    new achievement_with_open_arms("achievement_with_open_arms");
    new achievement_with_open_arms("achievement_with_open_arms_25");
    new achievement_disarmed("achievement_disarmed");
    new achievement_disarmed("achievement_disarmed_25");
}
