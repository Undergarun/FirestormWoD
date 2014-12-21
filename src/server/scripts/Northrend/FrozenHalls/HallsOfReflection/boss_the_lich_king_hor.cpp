/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
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

#include "ScriptPCH.h"
#include "halls_of_reflection.h"
#include "ScriptedEscortAI.h"

enum Spells
{
    SPELL_WINTER                       = 69780,
    SPELL_PAIN_AND_SUFFERING           = 74115,
    SPELL_FURY_OF_FROSTMOURNE          = 70063,
    SPELL_SOUL_REAPER                  = 73797,
    SPELL_RAISE_DEAD                   = 69818,
    SPELL_ICE_PRISON                   = 69708,
    SPELL_DARK_ARROW                   = 70194,
    SPELL_HARVEST_SOUL                 = 70070,

    // Raging gnoul
    SPELL_EMERGE_VISUAL                = 50142,
    SPELL_GNOUL_JUMP                   = 70150,

    // Witch Doctor
    SPELL_COURSE_OF_DOOM               = 70144,
    H_SPELL_COURSE_OF_DOOM             = 70183,
    SPELL_SHADOW_BOLT_VOLLEY           = 70145,
    H_SPELL_SHADOW_BOLT_VOLLEY         = 70184,
    SPELL_SHADOW_BOLT                  = 70080,
    H_SPELL_SHADOW_BOLT                = 70182,

    //Lumbering Abomination
    SPELL_ABON_STRIKE                  = 40505,
    SPELL_VOMIT_SPRAY                  = 70176,
    H_SPELL_VOMIT_SPRAY                = 70181,
};

enum Yells
{
    SAY_LICH_KING_WALL_01              = 2,
    SAY_LICH_KING_WALL_02              = 3,
    SAY_LICH_KING_WALL_03              = 4,
    SAY_LICH_KING_WALL_04              = 5,
    SAY_LICH_KING_GNOUL                = 6,
    SAY_LICH_KING_ABON                 = 7,
    SAY_LICH_KING_WINTER               = 8,
    SAY_LICH_KING_END_DUN              = 9,
    SAY_LICH_KING_WIN                  = 10,
};

class boss_lich_king_hor : public CreatureScript
{
    public:
        boss_lich_king_hor() : CreatureScript("boss_lich_king_hor") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_lich_king_horAI(creature);
        }

        struct boss_lich_king_horAI : public npc_escortAI
        {
            boss_lich_king_horAI(Creature* creature) : npc_escortAI(creature)
            {
                _instance = creature->GetInstanceScript();
                Reset();
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
            }

            InstanceScript* _instance;
            uint32 Step;
            uint32 StepTimer;
            uint32 uiWall;
            bool StartEscort;
            bool NonFight;

            void Reset()
            {
                NonFight = false;
                StartEscort = false;
                uiWall = 0;
            }

            void DamageTaken(Unit* /*who*/, uint32 &damage)
            {
                damage = 0;
            }

            void WaypointReached(uint32 i)
            {
                if (!_instance)
                    return;

                if (_instance->GetData(DATA_ICE_WALL_STATE_1) == IN_PROGRESS)
                {
                    uiWall = 1;
                    SetEscortPaused(true);
                }

                if (_instance->GetData(DATA_ICE_WALL_STATE_2) == IN_PROGRESS)
                {
                    uiWall = 2;
                    SetEscortPaused(true);
                }

                if (_instance->GetData(DATA_ICE_WALL_STATE_3) == IN_PROGRESS)
                {
                    uiWall = 3;
                    SetEscortPaused(true);
                }

                if (_instance->GetData(DATA_ICE_WALL_STATE_4) == IN_PROGRESS)
                {
                    uiWall = 4;
                    SetEscortPaused(true);
                }

                switch (i)
                {
                    case 20:
                        SetEscortPaused(true);
                        _instance->SetData(DATA_LICHKING_EVENT, SPECIAL);
                        Talk(SAY_LICH_KING_END_DUN);
                        if (Creature* escapeLider = ObjectAccessor::GetCreature(*me, _instance ? _instance->GetData64(DATA_ESCAPE_LIDER) : 0))
                            me->CastSpell(escapeLider, SPELL_HARVEST_SOUL, false);
                        me->setActive(false);
                        break;
                }
            }

            void AttackStart(Unit* who)
            {
                if (!_instance || !who || NonFight)
                    return;

                if (_instance->GetData(DATA_LICHKING_EVENT) == IN_PROGRESS || who->GetTypeId() == TYPEID_PLAYER)
                    return;

                me->Attack(who, true);
            }

            void JustSummoned(Creature* summoned)
            {
                if (!_instance || !summoned)
                    return;

                summoned->setActive(true);
                
                _instance->SetData(DATA_SUMMONS, 1);
            }

            void CallGuard(uint32 GuardID)
            {
                me->SummonCreature(GuardID,(me->GetPositionX()-5)+rand()%10, (me->GetPositionY()-5)+rand()%10, me->GetPositionZ(),4.17f,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,360000);
            }

            void Wall01()
            {
                switch (Step)
                {
                    case 0:
                        _instance->SetData(DATA_SUMMONS, 3);
                        Talk(SAY_LICH_KING_WALL_01);
                        StepTimer = 2000;
                        ++Step;
                        break;
                    case 1:
                        DoCast(me, SPELL_RAISE_DEAD);
                        Talk(SAY_LICH_KING_GNOUL);
                        StepTimer = 7000;
                        ++Step;
                        break;
                    case 2:
                        DoCast(me, SPELL_PAIN_AND_SUFFERING);
                        DoCast(me, SPELL_WINTER);
                        Talk(SAY_LICH_KING_WINTER);
                        StepTimer = 1000;
                        ++Step;
                        break;
                    case 3:
                        StepTimer = 2000;
                        ++Step;
                        break;
                    case 4:
                        me->SetSpeed(MOVE_WALK, 0.7f);
                        CallGuard(NPC_RISEN_WITCH_DOCTOR);
                        _instance->SetData(DATA_ICE_WALL_STATE_1, DONE);
                        StepTimer = 100;
                        Step = 0;
                        uiWall = 0;
                        SetEscortPaused(false);
                        break;
                }
            }

            void Wall02()
            {
                switch (Step)
                {
                    case 0:
                        _instance->SetData(DATA_SUMMONS, 3);
                        Talk(SAY_LICH_KING_WALL_02);
                        StepTimer = 2000;
                        ++Step;
                        break;
                    case 1:
                        Talk(SAY_LICH_KING_GNOUL);
                        DoCast(me, SPELL_RAISE_DEAD);
                        StepTimer = 6000;
                        ++Step;
                        break;
                    case 2:
                        CallGuard(NPC_RISEN_WITCH_DOCTOR);
                        CallGuard(NPC_RISEN_WITCH_DOCTOR);
                        CallGuard(NPC_ABON);
                        _instance->SetData(DATA_ICE_WALL_STATE_2, DONE);
                        StepTimer = 5000;
                        Step = 0;
                        uiWall = 0;
                        SetEscortPaused(false);
                        break;
                }
            }

            void Wall03()
            {
                switch(Step)
                {
                    case 0:
                        _instance->SetData(DATA_SUMMONS, 3);
                        Talk(SAY_LICH_KING_WALL_03);
                        StepTimer = 2000;
                        ++Step;
                        break;
                    case 1:
                        DoCast(me, SPELL_RAISE_DEAD);
                        Talk(SAY_LICH_KING_GNOUL);
                        StepTimer = 6000;
                        ++Step;
                        break;
                    case 2:
                        Talk(SAY_LICH_KING_ABON);
                        CallGuard(NPC_RISEN_WITCH_DOCTOR);
                        CallGuard(NPC_RISEN_WITCH_DOCTOR);
                        CallGuard(NPC_RISEN_WITCH_DOCTOR);
                        CallGuard(NPC_ABON);
                        CallGuard(NPC_ABON);
                        _instance->SetData(DATA_ICE_WALL_STATE_3, DONE);
                        StepTimer = 5000;
                        Step = 0;
                        uiWall = 0;
                        SetEscortPaused(false);
                        break;
                }
            }

            void Wall04()
            {
                switch(Step)
                {
                    case 0:
                        _instance->SetData(DATA_SUMMONS, 3);
                        Talk(SAY_LICH_KING_WALL_04);
                        StepTimer = 2000;
                        ++Step;
                        break;
                    case 1:
                        DoCast(me, SPELL_RAISE_DEAD);
                        Talk(SAY_LICH_KING_GNOUL);
                        StepTimer = 6000;
                        ++Step;
                        break;
                    case 2:
                        CallGuard(NPC_RISEN_WITCH_DOCTOR);
                        CallGuard(NPC_RISEN_WITCH_DOCTOR);
                        CallGuard(NPC_RISEN_WITCH_DOCTOR);
                        CallGuard(NPC_ABON);
                        CallGuard(NPC_ABON);
                        StepTimer = 15000;
                        ++Step;
                        break;
                    case 3:
                        Talk(SAY_LICH_KING_ABON);
                        CallGuard(NPC_RISEN_WITCH_DOCTOR);
                        CallGuard(NPC_RISEN_WITCH_DOCTOR);
                        _instance->SetData(DATA_ICE_WALL_STATE_4, DONE);
                        uiWall = 0;
                        SetEscortPaused(false);
                        ++Step;
                        break;
                }
            }

            void UpdateEscortAI(const uint32 diff)
            {
                if (!_instance)
                    return;

                if (_instance->GetData(DATA_LICHKING_EVENT) == NOT_STARTED || _instance->GetData(DATA_LICHKING_EVENT) == FAIL)
                {
                    if (!UpdateVictim())
                        return;

                    DoMeleeAttackIfReady();
                }

                // Start chase for leader
                if (_instance->GetData(DATA_LICHKING_EVENT) == IN_PROGRESS && StartEscort != true)
                {
                    StartEscort = true;
                    me->RemoveAurasDueToSpell(SPELL_ICE_PRISON);
                    me->RemoveAurasDueToSpell(SPELL_DARK_ARROW);
                    me->SetInCombatWithZone();
                    me->setActive(true);
                    NonFight = true;
                    me->AttackStop();
                    me->SetSpeed(MOVE_WALK, 2.5f, true);
                    Start(false, false);
                    Step = 0;
                    StepTimer = 100;
                }

                // Leader caught, wipe
                if (Creature* escapeLider = ObjectAccessor::GetCreature(*me, _instance ? _instance->GetData64(DATA_ESCAPE_LIDER) : 0))
                {
                    if (escapeLider->IsWithinDistInMap(me, 2.0f) && _instance->GetData(DATA_LICHKING_EVENT) == IN_PROGRESS)
                    {
                        me->setActive(false);
                        SetEscortPaused(false);
                        me->StopMoving();
                        Talk(SAY_LICH_KING_WIN);
                        me->CastSpell(me, SPELL_FURY_OF_FROSTMOURNE, false);
                        me->DealDamage(escapeLider, escapeLider->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    }
                }

                if (uiWall == 1)
                {
                    if (StepTimer < diff)
                        Wall01();
                    else
                        StepTimer -= diff;
                }

                if (uiWall == 2)
                {
                    if (StepTimer < diff)
                        Wall02();
                    else
                        StepTimer -= diff;
                }

                if (uiWall == 3)
                {
                    if (StepTimer < diff)
                        Wall03();
                    else
                        StepTimer -= diff;
                }

                if (uiWall == 4)
                {
                    if (StepTimer < diff)
                        Wall04();
                    else
                        StepTimer -= diff;
                }
                return;
            }
        };
};

class npc_raging_gnoul : public CreatureScript
{
    public:
        npc_raging_gnoul() : CreatureScript("npc_raging_gnoul") { }

        struct npc_raging_gnoulAI : public ScriptedAI
        {
            npc_raging_gnoulAI(Creature *creature) : ScriptedAI(creature)
            {
                _instance = creature->GetInstanceScript();
            }

            InstanceScript* _instance;
            uint32 _emergeTimer;
            bool _doEmerge;
            bool _doJump;

            void Reset()
            {
                _emergeTimer = 4000;
                _doEmerge = false;
                _doJump = false;
            }

            void IsSummonedBy(Unit* /*owner*/)
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetStandState(UNIT_STAND_STATE_SUBMERGED);
                DoCast(me, SPELL_EMERGE_VISUAL); 
                DoZoneInCombat(me, 100.00f); 
            }

            void JustDied(Unit* /*killer*/)
            {
                if (_instance)
                    _instance->SetData(DATA_SUMMONS, 0);
            }

            void AttackStart(Unit* who)
            {
                if (!who || !_doEmerge)
                    return;

                ScriptedAI::AttackStart(who);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!_instance)
                    return;

                if (_instance->GetData(DATA_LICHKING_EVENT) == IN_PROGRESS)
                {
                    if (!_doEmerge)
                    {
                        if (_emergeTimer < diff)
                        {
                            me->SetStandState(UNIT_STAND_STATE_STAND);
                            _doEmerge = true;

                            me->SetReactState(REACT_AGGRESSIVE);
                            if (Unit* victim = me->SelectVictim())
                            {
                                DoResetThreat(); 
                                AttackStart(victim);
                            }
                        }
                        else
                            _emergeTimer -= diff;
                    }

                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f))
                    {
                        if (!_doJump && me->IsInRange(target, 5, 30, false))
                        {
                            _doJump = true;
                            DoCast(target, SPELL_GNOUL_JUMP);
                        }
                    }
                }
                else if (_instance->GetData(DATA_LICHKING_EVENT) == FAIL || _instance->GetData(DATA_LICHKING_EVENT) == NOT_STARTED)
                    me->DespawnOrUnsummon();

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_raging_gnoulAI(creature);
        }
};

class npc_risen_witch_doctor : public CreatureScript
{
    public:
        npc_risen_witch_doctor() : CreatureScript("npc_risen_witch_doctor") { }

        struct npc_risen_witch_doctorAI : public ScriptedAI
        {
            npc_risen_witch_doctorAI(Creature *creature) : ScriptedAI(creature)
            {
                _instance = creature->GetInstanceScript();
            }

            InstanceScript* _instance;
            uint32 _emergeTimer;
            bool _doEmerge;
            uint32 _boltTimer;
            uint32 _boltVolleyTimer;
            uint32 _curseTimer;

            void Reset()
            {
                _emergeTimer = 5000;
                _boltTimer = 6000;
                _boltVolleyTimer = 15000;
                _curseTimer = 7000;
                _doEmerge = false;
            }

            void IsSummonedBy(Unit* /*owner*/)
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetStandState(UNIT_STAND_STATE_SUBMERGED);
                DoCast(me, SPELL_EMERGE_VISUAL); 
                DoZoneInCombat(me, 100.00f); 
            }

            void JustDied(Unit* /*killer*/)
            {
                if (_instance)
                    _instance->SetData(DATA_SUMMONS, 0);
            }

            void AttackStart(Unit* who)
            {
                if (!who || !_doEmerge)
                    return;

                ScriptedAI::AttackStart(who);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!_instance)
                    return;

                if (_instance->GetData(DATA_LICHKING_EVENT) == IN_PROGRESS)
                {
                    if (!_doEmerge)
                    {
                        if (_emergeTimer < diff)
                        {
                            me->SetStandState(UNIT_STAND_STATE_STAND);
                            _doEmerge = true;
                            
                            me->SetReactState(REACT_AGGRESSIVE);
                            if (Unit* victim = me->SelectVictim())
                            {
                                DoResetThreat(); 
                                AttackStart(victim);
                            }
                        }
                        else
                            _emergeTimer -= diff;
                    }

                    if (_curseTimer < diff)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_COURSE_OF_DOOM);
                        _curseTimer = urand(10000, 15000);
                    }
                    else
                        _curseTimer -= diff;

                    if (_boltTimer < diff)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                           DoCast(target, SPELL_SHADOW_BOLT);
                        _boltTimer = urand(2000, 3000);
                    }
                    else
                        _boltTimer -= diff;

                    if (_boltVolleyTimer < diff)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            DoCast(target, SPELL_SHADOW_BOLT_VOLLEY);
                        _boltVolleyTimer = urand(15000, 22000);
                    }
                    else
                        _boltVolleyTimer -= diff;

                }
                else if (_instance->GetData(DATA_LICHKING_EVENT) == FAIL || _instance->GetData(DATA_LICHKING_EVENT) == NOT_STARTED)
                    me->DespawnOrUnsummon();

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_risen_witch_doctorAI(creature);
        }
};

class npc_abon : public CreatureScript
{
    public:
        npc_abon() : CreatureScript("npc_abon") { }

        struct npc_abonAI : public ScriptedAI
        {
            npc_abonAI(Creature* creature) : ScriptedAI(creature)
            {
                _instance = creature->GetInstanceScript();
            }

            InstanceScript* _instance;
            bool _doWalk; 
            uint32 _strikeTimer;
            uint32 _vomitTimer;

            void Reset()
            {
                _doWalk = false;
                _vomitTimer = 15000;
                _strikeTimer = 6000;
            }

            void UpdateAI(uint32 const diff)
            {
                if (!_instance)
                    return;

                if (_instance->GetData(DATA_LICHKING_EVENT) == IN_PROGRESS)
                {
                    if (!_doWalk)
                    {
                        _doWalk = true;
                        
                        me->SetReactState(REACT_AGGRESSIVE);
                        if (Unit* victim = me->SelectVictim())
                        {
                            DoResetThreat(); 
                            AttackStart(victim);
                        }
                    }

                    if (_strikeTimer < diff)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            DoCast(target, SPELL_ABON_STRIKE);
                        _strikeTimer = urand(7000, 9000);
                    }
                    else
                        _strikeTimer -= diff;

                    if (_vomitTimer < diff)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            DoCast(target, SPELL_VOMIT_SPRAY);
                        _vomitTimer = urand(15000, 20000);
                    }
                    else
                        _vomitTimer -= diff;
                }
                else if (_instance->GetData(DATA_LICHKING_EVENT) == FAIL || _instance->GetData(DATA_LICHKING_EVENT) == NOT_STARTED)
                    me->DespawnOrUnsummon();
               
                DoMeleeAttackIfReady();
            }

            void JustDied(Unit* /*killer*/)
            {
                if (_instance)
                    _instance->SetData(DATA_SUMMONS, 0);
            }
       };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_abonAI(creature);
        }
};

void AddSC_boss_lich_king_hr()
{
    new boss_lich_king_hor();
    new npc_raging_gnoul();
    new npc_risen_witch_doctor();
    new npc_abon();
}
