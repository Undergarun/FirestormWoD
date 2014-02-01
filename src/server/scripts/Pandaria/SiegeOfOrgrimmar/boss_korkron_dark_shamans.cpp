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

enum eSays
{
    /*
    Earthbreaker Haromm yells: And fire!
    Earthbreaker Haromm yells: Anguished water, stagnant and poisonous!
    Earthbreaker Haromm yells: Bend to our will!
    Earthbreaker Haromm yells: Crumbling earth, plagued with rust!
    Earthbreaker Haromm yells: Storm!
    Earthbreaker Haromm yells: The blood of the True Horde lusts for power!
    Earthbreaker Haromm yells: The True Horde takes what it wants!
    Earthbreaker Haromm yells: Your skull will adorn my necklace.
    Earthbreaker Haromm says: The elements... what have we done...
    */
};

enum eSpells
{
    SPEll_ASHEN_WALL          = 144070, // Summons a wall of Ash Elementals
    SPEll_ASHFLAR_TOTEM       = 144290, // Grants the Falling Ash and Ashen Wall abilities.
    SPEll_BERSERK             = 47008,  // Increases the caster's attack speed by 150% and all damage it deals by 900% for 30 min.
    SPEll_BLOODLUST           = 144302, // The Dark Shaman go into a blood frenzy, increasing all damage dealt by 25% and haste by 25%.
    SPEll_FOUL_STREAM         = 144090, // Deals 300000 Nature damage to all enemies on a line.
    SPEll_FOULSTREAM_TOTEM    = 144289, // Grants the Foul Stream and Foul Geyser abilities.
    SPEll_FROSTSTORM_STRIKE   = 144215, // Deals 300000 Frost damage and causes the target to take 25% additional Froststorm Strike damage for 30 sec.  
    SPEll_IRON_TOMB           = 144328, // Inflicts 500000 Physical damage to players within 3 yards and creates an Iron Tomb
    SPEll_POISONMIST_TOTEM    = 144288, // Grants the Toxic Mist and Toxic Storm abilities
    SPEll_RUSTED_IRON_TOTEM   = 144291, // Grants the Iron Tomb and Iron Prison abilities.
    SPEll_TOXIC_MIST          = 144089  // Deals 80000 Nature damage every 3 sec. for 30 sec and causes growing toxicity in the target.
};

enum eEvents
{
};

/*
         phase 1
{
	SPELL_FROSTSTORM_STRIKE    =144215,
    SPELL_TOXIC_MIST           =144089,
	SPELL_TOXICITY             =144107
	SPELL_FOUL_STREAM          =144090,
	SPELL_ASHEN_WALL           =144070,
};
    
         phase 2
{
	SPELL_FROSTSTORM_BOLT   =144214,
    SPELL_TOXIC_STORM       =144005,
	SPELL_TOXIC_TORNADO     =144029,
	SPELL_FOUL_GEYSER       =143990,  
	SPELL_FALLING_ASH       =143973,
}
*/

class boss_earthbreaker_haromm : public CreatureScript
{
    public:
        boss_earthbreaker_haromm() : CreatureScript("boss_earthbreaker_haromm") { }

        struct boss_earthbreaker_harommAI : public BossAI
        {
            boss_earthbreaker_harommAI(Creature* creature) : BossAI(creature, DATA_KORKRON_DARK_SHAMANS)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_earthbreaker_harommAI(creature);
        }
};
                  
/*
				// Darkfang and Bloodclaw, wolves riding dark shamans, will fight fiercely to defend their masters.
				// Spawn Darkfang
                        switch (rand() % 4) {
                        case 0:
                            me->SummonCreature(NPC_DARKFANG, LR_X, LR_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 1:
                            me->SummonCreature(NPC_DARKFANG, LL_X, LL_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 2:
                            me->SummonCreature(NPC_DARKFANG , UR_X, UR_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 3:
                            me->SummonCreature(NPC_DARKFANG, UL_X, UL_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        }

						 // Spawn Bloodclaw
                        switch (rand() % 4) {
                        case 0:
                            me->SummonCreature(NPC_BLOODCLAW, LR_X, LR_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 1:
                            me->SummonCreature(NPC_BLOODCLAW , LL_X, LL_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 2:
                            me->SummonCreature(NPC_BLOODCLAW  , UR_X, UR_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 3:
                            me->SummonCreature(NPC_BLOODCLAW , UL_X, UL_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        }

						//During the battle, the dark shaman totems pose giving them new Spell
						 
						// Spawn Poisonmist_totem
                        switch (rand() % 4) {
                        case 0:
                            me->SummonCreature(NPC_POISONMIST_TOTEM, LR_X, LR_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 1:
                            me->SummonCreature(NPC_POISONMIST_TOTEM, LL_X, LL_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 2:
                            me->SummonCreature(NPC_POISONMIST_TOTEM, UR_X, UR_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 3:
                            me->SummonCreature(NPC_POISONMIST_TOTEM, UL_X, UL_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        }

						 // Spawn Foulstream_Totem
                        switch (rand() % 4) {
                        case 0:
                            me->SummonCreature(NPC_FOULSTREAM_TOTEM, LR_X, LR_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 1:
                            me->SummonCreature(NPC_FOULSTREAM_TOTEM, LL_X, LL_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 2:
                            me->SummonCreature(NPC_FOULSTREAM_TOTEM, UR_X, UR_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 3:
                            me->SummonCreature(NPC_FOULSTREAM_TOTEM, UL_X, UL_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        }

						// Spawn Ashflare_Totem
                        switch (rand() % 4) {
                        case 0:
                            me->SummonCreature(NPC_ASHFLARE_TOTEM, LR_X, LR_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 1:
                            me->SummonCreature(NPC_ASHFLARE_TOTEM, LL_X, LL_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 2:
                            me->SummonCreature(NPC_ASHFLARE_TOTEM, UR_X, UR_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 3:
                            me->SummonCreature(NPC_ASHFLARE_TOTEM, UL_X, UL_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        }
						 }

					    // Spawn Foul_Slim
                        switch (rand() % 4) {
                        case 0:
                            me->SummonCreature(NPC_FOUL_SLIME, LR_X, LR_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 1:
                            me->SummonCreature(NPC_FOUL_SLIME, LL_X, LL_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 2:
                            me->SummonCreature(NPC_FOUL_SLIME, UR_X, UR_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 3:
                            me->SummonCreature(NPC_FOUL_SLIME, UL_X, UL_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        }
*/

class boss_bloodclaw : public CreatureScript
{
    public:
        boss_bloodclaw() : CreatureScript("boss_bloodclaw") { }

        struct boss_bloodclawAI : public BossAI
        {
            boss_bloodclawAI(Creature* creature) : BossAI(creature, DATA_KORKRON_DARK_SHAMANS)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_bloodclawAI(creature);
        }
};

class mob_darkfang : public CreatureScript
{
    public:
        mob_darkfang() : CreatureScript("mob_darkfang") { }

        struct mob_darkfangAI : public ScriptedAI
        {
            mob_darkfangAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

		    void Reset()
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_darkfangAI(creature);
        }
};

class mob_foul_slime : public CreatureScript
{
    public:
        mob_foul_slime() : CreatureScript("mob_foul_slime") { }

        struct mob_foul_slimeAI : public ScriptedAI
        {
            mob_foul_slimeAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                pInstance = pCreature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;
            }
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new mob_foul_slimeAI(pCreature);
        }
};

void AddSC_korkron_dark_shamans()
{
    new boss_earthbreaker_haromm();
    new boss_bloodclaw();
    new mob_darkfang();
    new mob_foul_slime();
}
