#include "ScriptPCH.h"
#include "halls_of_origination.h"

enum ScriptTexts
{
    SAY_DEATH       = 0,
    SAY_AGGRO       = 1,
    SAY_SUPERNOVA   = 2,
    SAY_KILL        = 3, 
};

enum Spells
{
    SPELL_SUPERNOVA             = 74136,
    SPELL_ASTRAL_RAIN1          = 74134,
    SPELL_ASTRAL_RAIN2          = 74365,
    SPELL_ASTRAL_RAIN3          = 74371,
    SPELL_CELESTIAL_CALL1       = 74362,
    SPELL_CELESTIAL_CALL2       = 74355,
    SPELL_CELESTIAL_CALL3       = 74364,
    SPELL_VEIL_OF_SKY1          = 74133,
    SPELL_VEIL_OF_SKY2          = 74372,
    SPELL_VEIL_OF_SKY3          = 74373,
    SPELL_ARCANE_BARRAGE        = 74374,
    SPELL_ARCANE_BARRAGE_H      = 89886,
    SPELL_MIRROR_IMAGE_VS       = 74261, // summon veil of sky
    SPELL_MIRROR_IMAGE_AR       = 74262, // summon astral rain
    SPELL_MIRROR_IMAGE_CC       = 74263, // summon celestial call
};

enum Events
{
    EVENT_SUPERNOVA         = 1,
    EVENT_ASTRAL_RAIN       = 2,
    EVENT_CELESTIAL_CALL    = 3,
    EVENT_VEIL_OF_SKY       = 4,
    EVENT_ENTER_COMBAT      = 5, // for images
    EVENT_ARCANE_BARRAGE    = 7,
};

enum Adds
{
    NPC_ASTRAL_RAIN             = 39720, // 74265
    NPC_CELESTIAL_CALL          = 39721, // 74289
    NPC_VEIL_OF_SKY             = 39722,
    NPC_ASTRAL_FAMILIAR         = 39795,
    NPC_SPATIAL_FLUX            = 48707, // spawnmask=0
    NPC_ISISET_ADD_CONTROLLER   = 42382,
};

class boss_isiset : public CreatureScript
{
    public:
        boss_isiset() : CreatureScript("boss_isiset") { }

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_isisetAI(pCreature);
        }

        struct boss_isisetAI : public BossAI
        {
            boss_isisetAI(Creature* pCreature) : BossAI(pCreature, DATA_ISISET)
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
			    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
            }

            uint8 phase;
            bool Phased;

            bool AstralRain, VeilOfSky, CelestialCall;

            void InitializeAI()
            {
                if (!instance || static_cast<InstanceMap*>(me->GetMap())->GetScriptId() != sObjectMgr->GetScriptId(HOScriptName))
                    me->IsAIEnabled = false;
                else if (!me->isDead())
                    Reset();
            }

            void Reset()
            {
                _Reset();

                phase = 0;
                AstralRain = true;
                VeilOfSky = true;
                CelestialCall = true;

                me->SetVisible(true);
                me->SetReactState(REACT_AGGRESSIVE);
            }

            void EnterCombat(Unit *who)
            {
                Talk(SAY_AGGRO);

                events.ScheduleEvent(EVENT_SUPERNOVA, urand(7000, 10000));
                events.ScheduleEvent(EVENT_CELESTIAL_CALL, urand(9000, 15000));
                events.ScheduleEvent(EVENT_ASTRAL_RAIN, urand(10000, 17000));
                events.ScheduleEvent(EVENT_VEIL_OF_SKY, urand(6000, 10000));

                instance->SetBossState(DATA_ISISET, IN_PROGRESS);
            }

            void KilledUnit(Unit* victim)
            {
                Talk(SAY_KILL);
            }

            void JustDied(Unit* Killer)
            {
                _JustDied();
                Talk(SAY_DEATH);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                BossAI::SummonedCreatureDespawn(summon);

                if (phase == 1 || phase == 3)
                {
                    switch (summon->GetEntry())
                    {
                        case NPC_ASTRAL_RAIN:
                            AstralRain = false;
                            phase++;
                            summons.DespawnEntry(NPC_CELESTIAL_CALL);
                            summons.DespawnEntry(NPC_VEIL_OF_SKY);
                            me->SetVisible(true);
                            me->SetReactState(REACT_AGGRESSIVE);
                            events.ScheduleEvent(EVENT_SUPERNOVA, urand(7000, 10000));
                            events.ScheduleEvent(EVENT_CELESTIAL_CALL, urand(9000, 15000));
                            events.ScheduleEvent(EVENT_ASTRAL_RAIN, urand(10000, 17000));
                            events.ScheduleEvent(EVENT_VEIL_OF_SKY, urand(6000, 10000));
                            break;
                        case NPC_CELESTIAL_CALL:
                            CelestialCall = false;
                            phase++;
                            summons.DespawnEntry(NPC_ASTRAL_RAIN);
                            summons.DespawnEntry(NPC_VEIL_OF_SKY);
                            me->SetVisible(true);
                            me->SetReactState(REACT_AGGRESSIVE);
                            events.ScheduleEvent(EVENT_SUPERNOVA, urand(7000, 10000));
                            events.ScheduleEvent(EVENT_CELESTIAL_CALL, urand(9000, 15000));
                            events.ScheduleEvent(EVENT_ASTRAL_RAIN, urand(10000, 17000));
                            events.ScheduleEvent(EVENT_VEIL_OF_SKY, urand(6000, 10000));
                            break;
                        case NPC_VEIL_OF_SKY:
                            VeilOfSky = false;
                            phase++;
                            summons.DespawnEntry(NPC_CELESTIAL_CALL);
                            summons.DespawnEntry(NPC_ASTRAL_RAIN);
                            me->SetVisible(true);
                            me->SetReactState(REACT_AGGRESSIVE);
                            events.ScheduleEvent(EVENT_SUPERNOVA, urand(7000, 10000));
                            events.ScheduleEvent(EVENT_CELESTIAL_CALL, urand(9000, 15000));
                            events.ScheduleEvent(EVENT_ASTRAL_RAIN, urand(10000, 17000));
                            events.ScheduleEvent(EVENT_VEIL_OF_SKY, urand(6000, 10000));
                            break;
                    }
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if ((me->HealthBelowPct(67) && phase == 0) ||
                    (me->HealthBelowPct(34) && phase == 2))
                {
                    phase++;
                    events.Reset();
                    me->AttackStop();
                    me->SetReactState(REACT_PASSIVE);
                    if (CelestialCall)
                         DoCast(me, SPELL_MIRROR_IMAGE_CC);
                    if (AstralRain)    
                        DoCast(me, SPELL_MIRROR_IMAGE_AR);
                    if (VeilOfSky)
                        DoCast(me, SPELL_MIRROR_IMAGE_VS);
                    me->SetVisible(false);
                    return;
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
				    return;

			    while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SUPERNOVA:
                            Talk(SAY_SUPERNOVA);
                            DoCastAOE(SPELL_SUPERNOVA);
                            events.ScheduleEvent(EVENT_SUPERNOVA, urand(20000, 25000));
                            break;
                        case EVENT_ASTRAL_RAIN:
                            if (AstralRain)
                            {
                                if (phase == 0) DoCastAOE(SPELL_ASTRAL_RAIN1);
                                else if (phase == 2) DoCastAOE(SPELL_ASTRAL_RAIN2);
                                else if (phase == 4) DoCastAOE(SPELL_ASTRAL_RAIN3);
                                events.ScheduleEvent(EVENT_ASTRAL_RAIN, urand(15000, 22000));
                            }
                            break;
                        case EVENT_CELESTIAL_CALL:
                            if (CelestialCall)
                            {
                                if (phase == 0) DoCast(me, SPELL_CELESTIAL_CALL1);
                                else if (phase == 2) DoCast(me, SPELL_CELESTIAL_CALL2);
                                else if (phase == 4) DoCast(me, SPELL_CELESTIAL_CALL3);
                                events.ScheduleEvent(EVENT_CELESTIAL_CALL, urand(19000, 24000));
                            }
                            break;
                        case EVENT_VEIL_OF_SKY:
                            if (VeilOfSky)
                            {
                                if (phase == 0) DoCast(me, SPELL_VEIL_OF_SKY1);
                                else if (phase == 2) DoCast(me, SPELL_VEIL_OF_SKY2);
                                else if (phase == 4) DoCast(me, SPELL_VEIL_OF_SKY3);
                                events.ScheduleEvent(EVENT_VEIL_OF_SKY, urand(25000, 27000));
                            }
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_isiset_astral_rain : public CreatureScript
{
    public:
        npc_isiset_astral_rain() : CreatureScript("npc_isiset_astral_rain") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_isiset_astral_rainAI(creature);
        }

        struct npc_isiset_astral_rainAI : public ScriptedAI
        {
            npc_isiset_astral_rainAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
			    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
                me->SetReactState(REACT_PASSIVE);
                pInstance = pCreature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_ASTRAL_RAIN, urand(5000, 15000));
                events.ScheduleEvent(EVENT_ENTER_COMBAT, 2000);
            }

            void JustDied(Unit* killer)
            {
                me->DespawnOrUnsummon();
            }

            void UpdateAI(const uint32 diff)
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
                        case EVENT_ENTER_COMBAT:
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->GetMotionMaster()->MoveChase(me->getVictim());
                            break;
                        case EVENT_ASTRAL_RAIN:
                            DoCast(me, SPELL_ASTRAL_RAIN1);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_isiset_celestial_call : public CreatureScript
{
    public:
        npc_isiset_celestial_call() : CreatureScript("npc_isiset_celestial_call") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_isiset_celestial_callAI(creature);
        }

        struct npc_isiset_celestial_callAI : public ScriptedAI
        {
            npc_isiset_celestial_callAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
			    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
                me->SetReactState(REACT_PASSIVE);
                pInstance = pCreature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_CELESTIAL_CALL, urand(5000, 15000));
                events.ScheduleEvent(EVENT_ENTER_COMBAT, 2000);
            }

            void JustDied(Unit* killer)
            {
                me->DespawnOrUnsummon();
            }

            void UpdateAI(const uint32 diff)
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
                        case EVENT_ENTER_COMBAT:
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->GetMotionMaster()->MoveChase(me->getVictim());
                            break;
                        case EVENT_CELESTIAL_CALL:
                            DoCast(me, SPELL_CELESTIAL_CALL1);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_isiset_veil_of_sky : public CreatureScript
{
    public:
        npc_isiset_veil_of_sky() : CreatureScript("npc_isiset_veil_of_sky") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_isiset_veil_of_skyAI(creature);
        }

        struct npc_isiset_veil_of_skyAI : public ScriptedAI
        {
            npc_isiset_veil_of_skyAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
			    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
                me->SetReactState(REACT_PASSIVE);
                pInstance = pCreature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_VEIL_OF_SKY, urand(5000, 15000));
                events.ScheduleEvent(EVENT_ENTER_COMBAT, 2000);
            }

            void JustDied(Unit* killer)
            {
                me->DespawnOrUnsummon();
            }

            void UpdateAI(const uint32 diff)
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
                        case EVENT_ENTER_COMBAT:
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->GetMotionMaster()->MoveChase(me->getVictim());
                            break;
                        case EVENT_VEIL_OF_SKY:
                            DoCast(me, SPELL_VEIL_OF_SKY1);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_isiset_astral_familiar : public CreatureScript
{
    public:
        npc_isiset_astral_familiar() : CreatureScript("npc_isiset_astral_familiar") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_isiset_astral_familiarAI(creature);
        }

        struct npc_isiset_astral_familiarAI : public ScriptedAI
        {
            npc_isiset_astral_familiarAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
			    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
			    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_ARCANE_BARRAGE, urand(3000, 7000));
            }

            void JustDied(Unit* killer)
            {
                me->DespawnOrUnsummon();
            }

            void UpdateAI(const uint32 diff)
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
                        case EVENT_ARCANE_BARRAGE:
                            DoCastAOE(SPELL_ARCANE_BARRAGE);
                            events.ScheduleEvent(EVENT_ARCANE_BARRAGE, urand(8000, 15000));
                            break;
                    }
                }
            }
        };
};

class OrientationCheck
{
    public:
        explicit OrientationCheck(WorldObject* _caster) : caster(_caster) { }
        bool operator() (WorldObject* unit)
        {
            return !unit->isInFront(caster, 2.5f);
        }

    private:
        WorldObject* caster;
};

class spell_isiset_supernova_dis : public SpellScriptLoader
{
    public:
        spell_isiset_supernova_dis() : SpellScriptLoader("spell_isiset_supernova_dis") { }
        class spell_isiset_supernova_dis_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_isiset_supernova_dis_SpellScript);
            
            void FilterTargets(std::list<WorldObject*>& unitList)
            {
                unitList.remove_if(OrientationCheck(GetCaster()));
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_isiset_supernova_dis_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript *GetSpellScript() const
        {
            return new spell_isiset_supernova_dis_SpellScript();
        }
};

class spell_isiset_supernova_dmg : public SpellScriptLoader
{
    public:
        spell_isiset_supernova_dmg() : SpellScriptLoader("spell_isiset_supernova_dmg") { }
        class spell_isiset_supernova_dmg_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_isiset_supernova_dmg_SpellScript);
            
            void FilterTargets(std::list<WorldObject*>& unitList)
            {
                unitList.remove_if(OrientationCheck(GetCaster()));
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_isiset_supernova_dmg_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript *GetSpellScript() const
        {
            return new spell_isiset_supernova_dmg_SpellScript();
        }
};

void AddSC_boss_isiset()
{
    new boss_isiset();
    new npc_isiset_astral_rain();
    new npc_isiset_celestial_call();
    new npc_isiset_veil_of_sky();
    new npc_isiset_astral_familiar();
    new spell_isiset_supernova_dis();
    new spell_isiset_supernova_dmg();
}
