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

/*
         phase 1 
{ 
  SPELL_HEROIC_SHOCKWAVE    =143716, 
  SPELL_AFRTERSHOCK         =143712 
  SPELL_KOR_'_KRON_BANNER   =143501 
  SPELL_RAVAGER             =143872, 
  PELL_COOLING_OFF          =143484, 
} 

                    // Spawn Orgrimmar_Faithul 
                        switch (rand() % 4) { 
                        case 0: 
                            me->SummonCreature(NPC_ORGRIMMAR_FAITHFUL , LR_X, LR_Y, 
                                    SPAWN_Z, 0, 
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 
                                    60000); 
                            break; 
                        case 1: 
                            me->SummonCreature(NPC_ORGRIMMAR_FAITHFUL , LL_X, LL_Y, 
                                    SPAWN_Z, 0, 
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 
                                    60000); 
                            break; 
                        case 2: 
                            me->SummonCreature(NPC_ORGRIMMAR_FAITHFUL, UR_X, UR_Y, 
                                    SPAWN_Z, 0, 
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 
                                    60000); 
                            break; 
                        case 3: 
                            me->SummonCreature(NPC_ORGRIMMAR_FAITHFUL, UL_X, UL_Y, 
                                    SPAWN_Z, 0, 
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 
                                    60000); 
                            break; 
                        } 
  
  
                        // Spawn Ironblade 
                        switch (rand() % 4) { 
                        case 0: 
                            me->SummonCreature(NPC_KOR_'_KRON_IRONBLADE, LR_X, LR_Y, 
                                    SPAWN_Z, 0, 
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 
                                    60000); 
                            break; 
                        case 1: 
                            me->SummonCreature(NPC_KOR_'_KRON_IRONBLADE, LL_X, LL_Y, 
                                    SPAWN_Z, 0, 
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 
                                    60000); 
                            break; 
                        case 2: 
                            me->SummonCreature(NPC_KOR_'_KRON_IRONBLADE , UR_X, UR_Y, 
                                    SPAWN_Z, 0, 
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 
                                    60000); 
                            break; 
                        case 3: 
                            me->SummonCreature(NPC_KOR_'_KRON_IRONBLADE, UL_X, UL_Y, 
                                    SPAWN_Z, 0, 
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 
                                    60000); 
                            break; 
                        } 
  
                        // Spawn Arcweaver 
                        switch (rand() % 4) { 
                        case 0: 
                            me->SummonCreature(NPC_KOR_'_KRON_ARCWEAVER  , LR_X, LR_Y, 
                                    SPAWN_Z, 0, 
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 
                                    60000); 
                            break; 
                        case 1: 
                            me->SummonCreature(NPC_KOR_'_KRON_ARCWEAVER , LL_X, LL_Y, 
                                    SPAWN_Z, 0, 
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 
                                    60000); 
                            break; 
                        case 2: 
                            me->SummonCreature(NPC_KOR_'_KRON_ARCWEAVER , UR_X, UR_Y, 
                                    SPAWN_Z, 0, 
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 
                                    60000); 
                            break; 
                        case 3: 
                            me->SummonCreature(NPC_KOR_'_KRON_ARCWEAVER  , UL_X, UL_Y, 
                                    SPAWN_Z, 0, 
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 
                                    60000); 
                            break; 
                        } 
  
                        // Spawn Assassin 
                        switch (rand() % 4) { 
                        case 0: 
                            me->SummonCreature(NPC_KOR_'_KRON_ASSASSIN , LR_X, LR_Y, 
                                    SPAWN_Z, 0, 
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 
                                    60000); 
                            break; 
                        case 1: 
                            me->SummonCreature(NPC_KOR_'_KRON_ASSASSIN , LL_X, LL_Y, 
                                    SPAWN_Z, 0, 
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 
                                    60000); 
                            break; 
                        case 2: 
                            me->SummonCreature(NPC_KOR_'_KRON_ASSASSIN , UR_X, UR_Y, 
                                    SPAWN_Z, 0, 
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 
                                    60000); 
                            break; 
                        case 3: 
                            me->SummonCreature(NPC_KOR_'_KRON_ASSASSIN , UL_X, UL_Y, 
                                    SPAWN_Z, 0, 
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 
                                    60000); 
                            break; 
                        } 
  
                        // Spawn Warshaman 
                        switch (rand() % 4) { 
                        case 0: 
                            me->SummonCreature(NPC_KOR_'_KRON_WARSHAMAN, LR_X, LR_Y, 
                                    SPAWN_Z, 0, 
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 
                                    60000); 
                            break; 
                        case 1: 
                            me->SummonCreature(NPC_KOR_'_KRON_WARSHAMAN, LL_X, LL_Y, 
                                    SPAWN_Z, 0, 
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 
                                    60000); 
                            break; 
                        case 2: 
                            me->SummonCreature(NPC_KOR_'_KRON_WARSHAMAN, UR_X, UR_Y, 
                                    SPAWN_Z, 0, 
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 
                                    60000); 
                            break; 
                        case 3: 
                            me->SummonCreature(NPC_KOR_'_KRON_WARSHAMAN, UL_X, UL_Y, 
                                    SPAWN_Z, 0, 
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 
                                    60000); 
                            break; 
                        } 
*/

enum eSpells
{
    SPELL_BATTLE_STANCE       = 143589, // A balanced stance that generates 1 rage every 1 sec. 
    SPELL_BERSERK             = 47008,  // Increases the caster's attack speed by 150% and all damage it deals by 900% for 30 min. 
    SPELL_BERSERKER_STANCE    = 143594, // A wild stance that increases damage dealt by 25%, damage taken by 25%, and rage generation by 100%. 
    SPELL_BONECRACKER         = 143638, // Crack the target's bones, reducing maximum health by 10% and causing the target to bleed for 40000 Physical damage every 1 sec. 
    SPELL_COOLING_OFF         = 143484, // General Nazgrim needs to cool down after using a rage-dependent ability, and is unable to use additional abilities that cost rage for 15 sec.  
    SPELL_DEFENSIVE_STANCE    = 143593, // A defensive stance that reduces damage taken by 10% and grants rage when struck. 
    SPELL_EXECUTE             = 143502, // Executes the enemy for 3000000 Physical damage. This attack pierces immunities and cannot be dodged, blocked, or parried. 
    SPELL_HEROIC_SHOCKWAVE    = 143716, // Inflicts 300000 Physical damage within 10 yards of the impact location. 
    SPELL_RAVAGER             = 143872, // Throw a whirling blade, inflicting  370000 to 430000 Physical damage to enemies within 6 yards every 1 sec. and granting General Nazgrim 5 rage for each enemy struck. 
    SPELL_SUNDERING_BLOW      = 143494, // Inflicts 600000 Physical damage and sunders the target's armor, reducing armor by 10% for 30 sec. Generates 5 rage per stack of Sundering Blow on the target. 
    SPELL_WAR_SONG            = 143503  // A powerful war cry inflicts 50% of enemies' maximum health as damage. 
};

enum eEvents
{
};

enum eSays
{
    /*
    General Nazgrim yells: All Kor'kron... under my command... kill them... NOW!
    General Nazgrim yells: Defend the gate!
    General Nazgrim yells: Don't be foolish, Vol'jin! You have no siege weapons left! You cannot win this battle!
    General Nazgrim yells: I am Nazgrim, fist of the Warchief and General of the Horde Army. If I must die today, I will die with honor, in battle, for the horde!
    General Nazgrim yells: It is an honor to finally face you in combat. Lok'tar Ogar, for Honor, and for the Horde!
    General Nazgrim yells: Kor'kron, at my side!
    General Nazgrim yells: Next squad, to the front!
    General Nazgrim yells: No alliance hand has ever struck me down. Do you think you are seasoned enough?
    General Nazgrim yells: Rally the forces!
    General Nazgrim yells: So, it has come to this. Together, we have learned and grown over the years, and now we find ourselves face to face on the battlefield. Do not think I will go easy on you, nor do I expect any quarter. What we do now, we do for the horde, both of us.
    General Nazgrim yells: Warriors, on the double!
    General Nazgrim yells: What? Kor'kron, to the gates! Now!
    General Nazgrim yells: You may have defeated my war machine, but the gate still stands. You will not take Orgrimmar today.
    General Nazgrim yells: You should have trained harder.
    General Nazgrim says: Ah... you have learned much... and learned well... an honorable battle.
    General Nazgrim says: I die... with honor... Lok'tar Ogar...
    General Nazgrim says: In the end, I stood by the warchief, because it was my duty, and I am glad that it was you who struck me down. May your strength... lead the horde... into a new era of prosperity...
    */
};

class boss_general_nazgrim : public CreatureScript
{
	public:
		boss_general_nazgrim() : CreatureScript("boss_general_nazgrim") { }

		struct boss_general_nazgrimAI : public BossAI
		{
			boss_general_nazgrimAI(Creature* creature) : BossAI(creature, DATA_GENERAL_NAZGRIM)
			{
				pInstance = creature->GetInstanceScript();
			}
			
			EventMap events;
			InstanceScript* pInstance;
			
			void Reset()
			{
				Reset();
				
				events.Reset();
				
				summons.DespawnAll();
				
				if (pInstance)
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
			}
			
			void JustReachedHome()
            {
                _JustReachedHome();

                if (pInstance)
                    pInstance->SetBossState(DATA_GENERAL_NAZGRIM, FAIL);
            }
			
			void EnterCombat(Unit* attacker)
            {
				// @TODO: Set in combat for other protectors
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    pInstance->SetBossState(DATA_GENERAL_NAZGRIM, IN_PROGRESS);
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
			
			void KilledUnit(Unit* who)
            {
            }
			
			void JustDied(Unit* killer)
            {
                _JustDied();

                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->SetBossState(DATA_GENERAL_NAZGRIM, DONE);
                }
            }
			
			void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);
			}
		};

        CreatureAI* GetAI(Creature* pCreature) const
		{
			return new boss_general_nazgrimAI(pCreature);
		}
};

class mob_orgrimmar_faithful : public CreatureScript
{
    public:
        mob_orgrimmar_faithful() : CreatureScript("mob_orgrimmar_faithful") { }

        struct mob_orgrimmar_faithfulAI: public ScriptedAI
        { 
            mob_orgrimmar_faithfulAI(Creature* creature) :  ScriptedAI(creature)
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
            return new mob_orgrimmar_faithfulAI(creature);
        }
};

class mob_korkron_ironblade : public CreatureScript
{
    public:
        mob_korkron_ironblade() : CreatureScript("mob_korkron_ironblade") { }

        struct mob_korkron_ironbladeAI : public ScriptedAI
        {
            mob_korkron_ironbladeAI(Creature* creature) : ScriptedAI(creature)
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
            return new mob_korkron_ironbladeAI(creature);
        }
};

class mob_korkron_arcweaver : public CreatureScript
{
    public:
        mob_korkron_arcweaver() : CreatureScript("mob_korkron_arcweaver") { }

        struct mob_korkron_arcweaverAI : public ScriptedAI
        {
            mob_korkron_arcweaverAI(Creature* creature) : ScriptedAI(creature)
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
            return new mob_korkron_arcweaverAI(creature);
        }
};

class mob_korkron_assassin : public CreatureScript
{
    public:
        mob_korkron_assassin() : CreatureScript("mob_korkron_assassin") { }

        struct mob_korkron_assassinAI : public ScriptedAI
        {
            mob_korkron_assassinAI(Creature* creature) : ScriptedAI(creature)
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
            return new mob_korkron_assassinAI(creature);
        }
};

class mob_korkron_warshaman : public CreatureScript
{
    public:
        mob_korkron_warshaman() : CreatureScript("mob_korkron_warshaman") { }

        struct mob_korkron_warshamanAI : public ScriptedAI
        {
            mob_korkron_warshamanAI(Creature* creature) : ScriptedAI(creature)
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
            return new mob_korkron_warshamanAI(creature);
        }
};

void AddSC_general_nazgrim()
{
	new boss_general_nazgrim();
    new mob_orgrimmar_faithful();
    new mob_korkron_ironblade();
    new mob_korkron_arcweaver();
    new mob_korkron_assassin();
    new mob_korkron_warshaman();
};