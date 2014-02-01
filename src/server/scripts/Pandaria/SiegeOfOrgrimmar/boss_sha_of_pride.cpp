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
#include "siege_of_orgrimmar.h"

enum spell
{
   SPELL_AURA_OF_PRIDE            = 146817, // Causes an enemy's pride to manifest as an aura around them, inflicting 250000 Shadow damage to their allies within 4.5 yards every 1 sec.
   SPELL_BANISHMENT               = 146823, // Enemies filled with pride are banished to a realm of corruption, leaving behind a physical manifestation of their pride. Once banished, they cannot escape until the physical form of their pride has been destroyed and suffer 125000 Shadow damage every 3 sec.
   SPELL_CORRUPTED_PRISON         = 144574, // Imprisons an enemy in a corrupted Sha prison, stunning them and inflicting 80000 Shadow damage every second.
   //SPELL_CORRUPTED_PRISON         = 144683, // Imprisons an enemy in a corrupted Sha prison, stunning them and inflicting 80000 Shadow damage every second.
   //SPELL_CORRUPTED_PRISON         = 144636, // Imprisons an enemy in a corrupted Sha prison, stunning them and inflicting 80000 Shadow damage every second.
   //SPELL_CORRUPTED_PRISON         = 144684, // Imprisons an enemy in a corrupted Sha prison, stunning them and inflicting 80000 Shadow damage every second.
   SPELL_IMPRISON                 = 144563, // Imprisons an enemy in a corrupted Sha prison, stunning them and inflicting 80000 Shadow damage every second.
   SPELL_MARK_OF_ARROGANCE        = 144351, // Marks an enemy, causing them to suffer 70000 Shadow damage every second for the remainder of the fight. This effect stacks.
   SPELL_NATURES_BARRIER          = 145379, // When Earth Shield heals a target, the target will gain 300% of the amount healed as an absorb.
   SPELL_PROJECTION               = 146822, // Your projection is about to explode!
   SPELL_REACHING_ATTACK          = 144774, // Strikes at a distant target, inflicting 50% of weapon damage as Shadow and increasing Shadow damage taken by 25% for 8 sec. This ability is used when no targets are in melee range.
   SPELL_SELF_REFLECTION          = 144800, // The Sha of Pride causes up to 5 players to reflect on their actions, creating a Reflection of Pride at their location. With each tick of Self-Reflection, the Sha focuses on players with higher levels of Pride, only targeting those with 25, 50 and then 75 Pride.
   SPELL_SWELLING_PRIDE           = 144400, // The Sha of Pride releases a wave of dark energy, inflicting 350000 Shadow damage to all players and trigger an additional effect on any player with 25 or more Pride.
   SPELL_UNLEASHED                = 144832, // The Sha is unleashsed, instantly killing Norushen. In addition, the unleashed power of the Sha inflicts 245000 Shadow damage every 10 seconds for the remainder of the encounter.
   SPELL_WOUNDED_PRIDE            = 144358  // Wounds an enemy, causing them to gain 5 Pride whenever they suffer melee damage from the Sha of Pride.
};

enum timer
{
};

enum text
{
    /*
    Sha of Pride yells: Blame your companions!
    Sha of Pride yells: Come, face me. Give in to your pride. Show me your "Greatness."
    Sha of Pride yells: So foolish...
    Sha of Pride yells: Succumb to your pride!
    Sha of Pride yells: That one is unworthy of your group.
    Sha of Pride yells: That one is WEAK.
    Sha of Pride yells: You are better than your companions!
    Sha of Pride yells: You should bow to no king or warchief.
    Sha of Pride yells: You've let your pride cloud your vision, Titan puppet! You can never contain ME!
    Sha of Pride yells: Your arrogance compels you...
    Sha of Pride yells: Your arrogance feeds me!
    */
};

enum creatures
{
    NPC_REFLECTION = 72172
};

/*
phase 1
{
    SPELL_MOCKING_BLAST      =144379,
    SPELL_LAST_WORLD         =144370,
    SPELL_SELF_REFLECTION    =144800,
    SPELL_CORRUPTED_PRISON   =144574,
    SPELL_REACHING_ATTACK    =119775,
    SPELL_UNLEASHED          =144832,
    SPELL_FINAL_GIFT         =144854
}
*/

class boss_sha_of_pride : public CreatureScript
{
    public:
        boss_sha_of_pride() : CreatureScript("boss_sha_of_pride") { }

        struct boss_sha_of_prideAI : public BossAI
        {
            boss_sha_of_prideAI(Creature* creature) : BossAI(creature, DATA_SHA_OF_PRIDE)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_sha_of_prideAI(creature);
        }
};

/*
                  // Handles spell casting. These spells only occur during phase 1
           if (phase == 1)
                if (uiMocking_BlastTimer <= diff) {
                     if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0)) {
						   DoCast(
                                pTarget,
                                RAID_MODE(SPELL_SPELL_MOCKING_BLAST_10,
                                        SPELL_SPELL_MOCKING_BLAST_25,SPELL_SPELL_MOCKING_BLAST_LFR,SPELL_SPELL_MOCKING_BLAST_FLEX));
                       uiMocking_BlastTarget = pTarget->GetGUID();
                    }
                    uiSpawnMocking_BlastTimer = TIMER_SPAWN_MOCKING_BLAST;
                    if (hardMode)
                        searing_light_active = true;
                    uiMocking_BlastTimer = TIMER_MOCKING_BLAST;
                } else
                    uiMocking_BlastTimer -= diff;

                if (uiLast_WorldTimer <= diff) {
                    if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0)) {
                        DoCast(
                                pTarget,
                                RAID_MODE(SPELL_LAST_WORLD_10,
                                        SPELL_LAST_WORLD_25,SPELL_LAST_WORLD_LFR,SPELL_LAST_WORLD_FLEX));
                        uiLast_WorldTarget = pTarget->GetGUID();
                    }
                    uiSpawnLast_WorldTimer = TIMER_SPAWN_LAST_WORLD;

                 if (uiSelf_ReflectionTimer <= diff) {
                    if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0)) {
                        DoCast(
                                pTarget,
                                RAID_MODE(SPELL_SELF_REFLECTION_10,
                                        SPELL_SELF_REFLECTION_25,SPELL_SELF_REFLECTION_LFR,SPELL_SELF_REFLECTION_FLEX));
                        uiSelf_ReflectionTarget = pTarget->GetGUID();
                    }
                    uiSpawnSelf_ReflectionTimer = TIMER_SPAWN_SELF_REFLECTION;


						 // Spawn REFLECTION
                        switch (rand() % 4) {
                        case 0:
                            me->SummonCreature(NPC_REFLECTION, LR_X, LR_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 1:
                            me->SummonCreature(NPC_REFLECTION, LL_X, LL_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 2:
                            me->SummonCreature(NPC_REFLECTION, UR_X, UR_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 3:
                            me->SummonCreature(NPC_REFLECTION, UL_X, UL_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;


                          if (uiCorrupted_prisonTimer <= diff) {
                    if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0)) {
                        DoCast(
                                pTarget,
                                RAID_MODE(SPELL_CORRUPTED_PRISON_10,
                                        SPELL_CORRUPTED_PRISON_25,SPELL_CORRUPTED_PRISON_LFR,SPELL_CORRUPTED_PRISON_FLEX));
                        uiCorrupted_prisonTarget = pTarget->GetGUID();
                    }
                    uiSpawnCorrupted_prisonTimer = TIMER_SPAWN_CORRUPTED_PRISON ;

                    if (uiReaching_AttackTimer <= diff) {
                    if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0)) {
                        DoCast(
                                pTarget,
                                RAID_MODE(SPELL_REACHING_ATTACK_10,
                                        SPELL_REACHING_ATTACK_25,SPELL_REACHING_ATTACK_LFR,SPELL_REACHING_ATTACK_FLEX));
                        uiReaching_AttackTarget = pTarget->GetGUID();
                    }
                    uiSpawnReaching_AttackTimer = TIMER_SPAWN_REACHING_ATTACK  ;

                      if (uiUnleashedTimer <= diff) {
                    if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0)) {
                        DoCast(
                                pTarget,
                                RAID_MODE(SPELL_UNLEASHED_10,
                                        SPELL_UNLEASHED_25,SPELL_UNLEASHED_LFR,SPELL_UNLEASHED_FLEX));
                        uiUnleashedTarget = pTarget->GetGUID();
                    }
                    uiSpawnUnleashedTimer = TIMER_SPAWN_UNLEASHED  ;

                     if (uiFinal_GiftTimer <= diff) {
                    if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0)) {
                        DoCast(
                                pTarget,
                                RAID_MODE(SPELL_FINAL_GIFT_10,
                                        SPELL_FINAL_GIFT_25,SPELL_FINAL_GIFT_LFR,SPELL_FINAL_GIFT_FLEX));
                        uiFinal_GiftTarget = pTarget->GetGUID();
                    }
                    uiSpawnFinal_GiftTimer = TIMER_SPAWN_FINAL_GIFT  ;
*/

class mob_reflection : public CreatureScript
{
    public:
        mob_reflection() : CreatureScript("mob_reflection") { }

        struct mob_reflectionAI : public ScriptedAI
        {
            mob_reflectionAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_reflectionAI(creature);
        }
};

void AddSC_sha_of_pride()
{
    new boss_sha_of_pride();
    new mob_reflection();
}
