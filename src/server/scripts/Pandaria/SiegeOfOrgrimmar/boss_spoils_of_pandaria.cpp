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

enum eSpells
{
    SPELL_GB_11010_ARMAGEDDON_CLASS_DEFENSE_SYSTEME = 145864,
    SPELL_UNSTABLE_DEFENSE_SYSTEME                  = 145685
};

enum eEvents
{
};

enum eSays
{
};

class boss_spoils_of_pandaria : public CreatureScript
{
    public:
        boss_spoils_of_pandaria() : CreatureScript("boss_spoils_of_pandaria") { }

        struct boss_spoils_of_pandariaAI : public BossAI
        {
            boss_spoils_of_pandariaAI(Creature* creature) : BossAI(creature, DATA_SPOILS_OF_PANDARIA)
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
                    pInstance->SetBossState(DATA_SPOILS_OF_PANDARIA, FAIL);
            }
            
            void EnterCombat(Unit* attacker)
            {
                // @TODO: Set in combat for other protectors
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    pInstance->SetBossState(DATA_SPOILS_OF_PANDARIA, IN_PROGRESS);
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
                    pInstance->SetBossState(DATA_SPOILS_OF_PANDARIA, DONE);
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
            return new boss_spoils_of_pandariaAI(pCreature);
        }
};

class mob_secured_stockpile_of_spoils : public CreatureScript
{
    public:
        mob_secured_stockpile_of_spoils() : CreatureScript("mob_secured_stockpile_of_spoils") { }

        struct mob_secured_stockpile_of_spoilsAI : public ScriptedAI
        {
            mob_secured_stockpile_of_spoilsAI(Creature* creature) : ScriptedAI(creature)
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
            return new mob_secured_stockpile_of_spoilsAI(creature);
        }
};

class mob_modified_anima_golem : public CreatureScript
{
    public:
        mob_modified_anima_golem() : CreatureScript("mob_modified_anima_golem") { }

        struct mob_modified_anima_golemAI : public ScriptedAI
        {
            mob_modified_anima_golemAI(Creature* creature) : ScriptedAI(creature)
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
        return new mob_modified_anima_golemAI(creature);
    }
};

class mob_mogu_shadow_ritualist : public CreatureScript
{
    public:
        mob_mogu_shadow_ritualist() : CreatureScript("mob_mogu_shadow_ritualist") { }

        struct mob_mogu_shadow_ritualistAI: public ScriptedAI
        {
            mob_mogu_shadow_ritualistAI(Creature* creature) : ScriptedAI(creature)
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
            return new mob_mogu_shadow_ritualistAI(creature);
        }
};

class mob_zarthik_amber_priest : public CreatureScript
{ 
    public:
        mob_zarthik_amber_priest() : CreatureScript("mob_zarthik_amber_priest") { }

        struct mob_zarthik_amber_priestAI : public ScriptedAI
        {
            mob_zarthik_amber_priestAI(Creature* creature) : ScriptedAI(creature)
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
            return new mob_zarthik_amber_priestAI(creature);
        }
};

class mob_setthik_wielder : public CreatureScript
{
    public:
        mob_setthik_wielder() : CreatureScript("mob_setthik_wielder") { }

        struct mob_setthik_wielderAI : public ScriptedAI
        {
            mob_setthik_wielderAI(Creature* creature) : ScriptedAI(creature)
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
            return new mob_setthik_wielderAI(creature);
        }
};

class mob_animated_stone_mogu : public CreatureScript
{
    public:
        mob_animated_stone_mogu() : CreatureScript("mob_animated_stone_mogu") { }

        struct mob_animated_stone_moguAI : public ScriptedAI
        {
            mob_animated_stone_moguAI(Creature* creature) : ScriptedAI(creature)
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
            return new mob_animated_stone_moguAI(creature);
        }
};

class mob_burial_urn : public CreatureScript
{
    public:
        mob_burial_urn() : CreatureScript("mob_burial_urn") { }

        struct mob_burial_urnAI : public ScriptedAI
        {
            mob_burial_urnAI(Creature* creature) : ScriptedAI(creature)
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
            return new mob_burial_urnAI(creature);
        }
};

class mob_quilen_gardians : public CreatureScript
{
    public:
        mob_quilen_gardians() : CreatureScript("mob_quilen_gardians") { }

        struct mob_quilen_gardiansAI : public ScriptedAI
        {
            mob_quilen_gardiansAI(Creature* creature) : ScriptedAI(creature)
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
            return new mob_quilen_gardiansAI(creature);
        }
};

class mob_srithik_bombardier : public CreatureScript
{
    public:
        mob_srithik_bombardier() : CreatureScript("mob_srithik_bombardier") { }

        struct mob_srithik_bombardierAI : public ScriptedAI
        {
            mob_srithik_bombardierAI(Creature* creature) : ScriptedAI(creature)
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
            return new mob_srithik_bombardierAI(creature);
        }
};

class mob_korthik_warcaller : public CreatureScript
{
    public:
        mob_korthik_warcaller() : CreatureScript("mob_korthik_warcaller") { }

        struct mob_korthik_warcallerAI : public ScriptedAI
        {
            mob_korthik_warcallerAI(Creature* creature) : ScriptedAI(creature)
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
            return new mob_korthik_warcallerAI(creature);
        }
};

class mob_ancient_breawmaster_spirit : public CreatureScript
{
    public:
        mob_ancient_breawmaster_spirit() : CreatureScript("mob_ancient_breawmaster_spirit") { }

        struct mob_ancient_breawmaster_spiritAI: public ScriptedAI
        {
            mob_ancient_breawmaster_spiritAI(Creature* creature) : ScriptedAI(creature)
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
            return new mob_ancient_breawmaster_spiritAI(creature);
        }
};

class mob_wise_misteweaver_spirit : public CreatureScript
{
    public:
        mob_wise_misteweaver_spirit() : CreatureScript("mob_wise_misteweaver_spirit") { }

        struct mob_wise_misteweaver_spiritAI : public ScriptedAI
        {
            mob_wise_misteweaver_spiritAI(Creature* creature) : ScriptedAI(creature)
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
            return new mob_wise_misteweaver_spiritAI(creature);
        }
};

class mob_namelesse_windwallker_spirit : public CreatureScript
{
    public:
        mob_namelesse_windwallker_spirit() : CreatureScript("mob_namelesse_windwallker_spirit") { } 

        struct mob_namelesse_windwallker_spiritAI : public ScriptedAI
        {
            mob_namelesse_windwallker_spiritAI(Creature* creature) : ScriptedAI(creature)
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
            return new mob_namelesse_windwallker_spiritAI(creature);
        }
};

void AddSC_spoils_of_pandaria()
{
    new boss_spoils_of_pandaria();
    new mob_secured_stockpile_of_spoils();
    new mob_modified_anima_golem();
    new mob_mogu_shadow_ritualist();
    new mob_zarthik_amber_priest();
    new mob_setthik_wielder();
    new mob_animated_stone_mogu();
    new mob_burial_urn();
    new mob_quilen_gardians();
    new mob_srithik_bombardier();
    new mob_korthik_warcaller();
    new mob_ancient_breawmaster_spirit();
    new mob_wise_misteweaver_spirit();
    new mob_namelesse_windwallker_spirit();
};