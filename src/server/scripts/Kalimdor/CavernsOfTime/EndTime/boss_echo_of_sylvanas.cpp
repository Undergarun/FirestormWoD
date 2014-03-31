#include "ScriptPCH.h"
#include "end_time.h"

enum ScriptTexts
{
    SAY_AGGRO   = 0,
    SAY_DEATH   = 1,
    SAY_KILL    = 3,
    SAY_WIPE    = 2,
    SAY_SPELL   = 4,
};

enum Spells
{
    SPELL_SHRIEK_OF_THE_HIGHBORNE       = 101412,
    SPELL_UNHOLY_SHOT                   = 101411,
    SPELL_BLACK_ARROW                   = 101404,
    SPELL_WRACKING_PAIN_ANY             = 100865,
    //                                = 100862,
    SPELL_WRACKING_PAIN_AURA            = 101258,
    SPELL_WRACKING_PAIN_DMG             = 101257,
    SPELL_DEATH_GRIP_AOE                = 101397,
    SPELL_DEATH_GRIP                    = 101987,
    SPELL_SUMMON_GHOULS                 = 102603, // before combat
    SPELL_TELEPORT                      = 101398, 
    SPELL_CALL_OF_THE_HIGHBORNE         = 100686, // immune
    SPELL_CALL_OF_THE_HIGHBORNE_1       = 100867, // visual spawn ghouls
    SPELL_CALL_OF_THE_HIGHBORNE_2       = 105766, // visual back ghouls
    SPELL_CALL_OF_THE_HIGHBORNE_3       = 102581, // visual ?
    SPELL_SPAWN_GHOUL                   = 101200,
    SPELL_SEEPING_SHADOWS_DUMMY         = 103175,
    SPELL_SEEPING_SHADOWS_AURA          = 103182,
    SPELL_SACRIFICE                     = 101348,
    SPELL_DWINDLE                       = 101259,
    SPELL_JUMP                          = 101517,
    SPELL_JUMP_SCRIPT                   = 101527,
    SPELL_JUMP_VEHICLE                  = 101528,
    SPELL_PERMANENT_FEIGH_DEATH         = 96733,
    SPELL_SHRINK                        = 101318,
};

enum Events
{
    EVENT_SHRIEK_OF_THE_HIGHBORNE   = 1,
    EVENT_UNHOLY_SHOT               = 2,
    EVENT_BLACK_ARROW               = 3,
    EVENT_DEATH_GRIP                = 4,
    EVENT_TELEPORT                  = 5,
    EVENT_TELEPORT_1                = 6,
    EVENT_SPAWN_GHOUL               = 7,
    EVENT_MOVE_GHOUL                = 8,
    EVENT_FALL                      = 0,
    EVENT_START                     = 10,
};

enum Adds
{
    NPC_GHOUL_1         = 54197,
    NPC_BRITTLE_GHOUL   = 54952,
    NPC_RISEN_GHOUL     = 54191,
    NPC_JUMP            = 54385,
};

enum Others
{
    DATA_GUID           = 1,
    POINT_GHOUL         = 2,
    ACTION_GHOUL        = 3,
    ACTION_KILL_GHOUL   = 4,
};

const Position centerPos = {3845.51f, 909.318f, 56.1463f, 1.309f};

const Position ghoulPos[8] =
{
    {3810.03f, 914.043f, 55.3974f, 0.0f},
    {3818.82f, 892.83f, 56.0076f, 0.7854f},
    {3840.03f, 884.043f, 56.0712f, 1.5708f},
    {3861.24f, 892.83f, 56.0712f, 2.35619f},
    {3870.03f, 914.043f, 56.0277f, 3.14159f},
    {3861.24f, 935.256f, 55.9664f, 3.92699f},
    {3840.03f, 944.043f, 55.9664f, 4.71239f},
    {3818.82f, 935.256f, 56.0161f, 5.49779f}
};

class boss_echo_of_sylvanas : public CreatureScript
{
    public:
        boss_echo_of_sylvanas() : CreatureScript("boss_echo_of_sylvanas") { }

         CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_echo_of_sylvanasAI(creature);
        }

        struct boss_echo_of_sylvanasAI : public BossAI
        {
            boss_echo_of_sylvanasAI(Creature* creature) : BossAI(creature, DATA_ECHO_OF_SYLVANAS)
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
                me->setActive(true);
                me->CastSpell(me, SPELL_SUMMON_GHOULS, true);
            }

            uint8 ghouls;
            uint8 deadghouls;

            void InitializeAI()
            {
                if (!instance || static_cast<InstanceMap*>(me->GetMap())->GetScriptId() != sObjectMgr->GetScriptId(ETScriptName))
                    me->IsAIEnabled = false;
                else if (!me->isDead())
                    Reset();
            }

            void Reset()
            {
                _Reset();
                me->SetReactState(REACT_AGGRESSIVE);
                me->SetDisableGravity(false);
                me->SetCanFly(false);
                ghouls = 0;
                deadghouls = 0;
            }

            void JustDied(Unit* killer)
            {
                _JustDied();
                Talk(SAY_DEATH);

                // Quest
                Map::PlayerList const &PlayerList = instance->instance->GetPlayers();
                if (!PlayerList.isEmpty())
                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                        if (Player* pPlayer = i->getSource())
                            if (me->GetDistance2d(pPlayer) <= 50.0f && pPlayer->GetQuestStatus(30097) == QUEST_STATUS_INCOMPLETE)
                                DoCast(pPlayer, SPELL_ARCHIVED_SYLVANAS, true);
            }

            void EnterCombat(Unit* /*who*/)
            {
                Talk(SAY_AGGRO);
                events.ScheduleEvent(EVENT_UNHOLY_SHOT, urand(5000, 20000));
                events.ScheduleEvent(EVENT_SHRIEK_OF_THE_HIGHBORNE, urand(5000, 20000));
                events.ScheduleEvent(EVENT_TELEPORT, 40000);
                me->RemoveAura(SPELL_SUMMON_GHOULS);
                summons.DespawnEntry(NPC_BRITTLE_GHOUL);
                deadghouls = 0;
                DoZoneInCombat();
                instance->SetBossState(DATA_ECHO_OF_SYLVANAS, IN_PROGRESS);
            }

            void KilledUnit(Unit* who)
            {
                if (who->GetTypeId() == TYPEID_PLAYER)
                    Talk(SAY_KILL);
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_GHOUL)
                {
                    ghouls++;
                    if (ghouls >= 1)
                    {
                        DoCastAOE(SPELL_SACRIFICE);
                        summons.DespawnEntry(NPC_RISEN_GHOUL);
                        events.ScheduleEvent(EVENT_START, 2000);
                        ghouls = 0;
                    }
                }
                else if (action == ACTION_KILL_GHOUL)
                    deadghouls++;
            }

            void JustReachedHome()
            {
                Talk(SAY_WIPE);
                DoCast(me, SPELL_SUMMON_GHOULS, true);
            }

            bool AllowAchieve()
            {
                return deadghouls >= 2; 
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
                        case EVENT_UNHOLY_SHOT:
                            if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(pTarget, SPELL_UNHOLY_SHOT);
                            events.ScheduleEvent(EVENT_UNHOLY_SHOT, urand(10000, 20000));
                            break;
                        case EVENT_SHRIEK_OF_THE_HIGHBORNE:
                            if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(pTarget, SPELL_SHRIEK_OF_THE_HIGHBORNE);
                            events.ScheduleEvent(EVENT_SHRIEK_OF_THE_HIGHBORNE, urand(15000, 21000));
                            break;
                        case EVENT_TELEPORT:
                            events.CancelEvent(EVENT_UNHOLY_SHOT);
                            events.CancelEvent(EVENT_SHRIEK_OF_THE_HIGHBORNE);
                            me->SetReactState(REACT_PASSIVE);
                            me->AttackStop();
                            me->InterruptNonMeleeSpells(false);
                            DoCast(me, SPELL_TELEPORT, true);
                            events.ScheduleEvent(EVENT_TELEPORT_1, 2000);
                            break;
                        case EVENT_TELEPORT_1:
                            Talk(SAY_SPELL);
                            DoCast(me, SPELL_CALL_OF_THE_HIGHBORNE, true);
                            for (uint8 i = 0; i < 8; ++i)
                                me->SummonCreature(NPC_GHOUL_1, ghoulPos[i]);
                            events.ScheduleEvent(EVENT_DEATH_GRIP, 3000);
                            events.ScheduleEvent(EVENT_SPAWN_GHOUL, 3000);
                            break;
                        case EVENT_DEATH_GRIP:
                            DoCastAOE(SPELL_DEATH_GRIP_AOE);
                            break;
                        case EVENT_SPAWN_GHOUL:
                        {
                            deadghouls = 0;
                            Unit* _first = NULL;
                            Unit* _prev = NULL;
                            for (uint8 i = 1; i < 8; ++i)
                            {
                                if (Creature* pGhoul = me->SummonCreature(NPC_RISEN_GHOUL, ghoulPos[i]))
                                {
                                    if (_prev)
                                    {
                                        _prev->CastSpell(pGhoul, SPELL_WRACKING_PAIN_ANY, true);
                                        _prev->GetAI()->SetGUID(pGhoul->GetGUID(), DATA_GUID);
                                        
                                    }
                                    _prev = pGhoul;
                                    if (i == 1)
                                        _first = pGhoul;
                                }
                            }
                            if (_first)
                            {
                                _prev->CastSpell(_first, SPELL_WRACKING_PAIN_ANY, true);
                                _prev->GetAI()->SetGUID(_first->GetGUID(), DATA_GUID);
                            }
                            summons.DespawnEntry(NPC_GHOUL_1);
                            break;
                        }
                        case EVENT_START:
                            me->RemoveAura(SPELL_CALL_OF_THE_HIGHBORNE);
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->GetMotionMaster()->MoveChase(me->getVictim());
                            events.ScheduleEvent(EVENT_UNHOLY_SHOT, urand(5000, 20000));
                            events.ScheduleEvent(EVENT_SHRIEK_OF_THE_HIGHBORNE, urand(5000, 20000)); 
                            events.ScheduleEvent(EVENT_TELEPORT, 40000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_echo_of_sylvanas_ghoul : public CreatureScript
{
    public:

        npc_echo_of_sylvanas_ghoul() : CreatureScript("npc_echo_of_sylvanas_ghoul") { }

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_echo_of_sylvanas_ghoulAI(pCreature);
        }

        struct npc_echo_of_sylvanas_ghoulAI : public Scripted_NoMovementAI
        {
            npc_echo_of_sylvanas_ghoulAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
            }

            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void IsSummonedBy(Unit* owner)
            {
                DoCast(me, SPELL_CALL_OF_THE_HIGHBORNE_1, true);
            }
        };
};

class npc_echo_of_sylvanas_risen_ghoul : public CreatureScript
{
    public:

        npc_echo_of_sylvanas_risen_ghoul() : CreatureScript("npc_echo_of_sylvanas_risen_ghoul") { }

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_echo_of_sylvanas_risen_ghoulAI(pCreature);
        }

        struct npc_echo_of_sylvanas_risen_ghoulAI : public Scripted_NoMovementAI
        {
            npc_echo_of_sylvanas_risen_ghoulAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
            {
                me->SetReactState(REACT_PASSIVE);
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

            EventMap events;
            uint64 _guid;

            void Reset()
            {
                events.Reset();
                _guid = 0;
                me->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 5.0f);
                me->SetFloatValue(UNIT_FIELD_COMBATREACH, 5.0f);
            }
 
            void JustDied(Unit* /*killer*/)
            {
                me->RemoveAura(SPELL_WRACKING_PAIN_ANY);
                if (Creature* pTarget = ObjectAccessor::GetCreature(*me, _guid))
                    pTarget->RemoveAura(SPELL_WRACKING_PAIN_ANY);
                _guid = 0;
                if (Creature* pSylvanas = me->FindNearestCreature(NPC_ECHO_OF_SYLVANAS, 300.0f))
                    pSylvanas->GetAI()->DoAction(ACTION_KILL_GHOUL);
                me->DespawnOrUnsummon(500);
            }

            void EnterCombat(Unit* /*who*/)
            {
                DoCast(me, SPELL_SEEPING_SHADOWS_DUMMY, true); 
                events.ScheduleEvent(EVENT_MOVE_GHOUL, 2000);
            }
            
            void SetGUID(uint64 guid, int32 type)
            {
                if (type == DATA_GUID)
                    _guid = guid;
            }

            uint64 GetGUID(int32 type)
            {
                if (type == DATA_GUID)
                    return _guid;
                return 0;
            }

            void MovementInform(uint32 type, uint32 data)
            {
                if (type == POINT_MOTION_TYPE)
                    if (data == POINT_GHOUL)
                        if (Unit* pSylvanas = me->FindNearestCreature(NPC_ECHO_OF_SYLVANAS, 300.0f))
                            pSylvanas->GetAI()->DoAction(ACTION_GHOUL);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_MOVE_GHOUL:
                            me->SetWalk(true);
                            me->SetSpeed(MOVE_RUN, 0.5f, true);
                            me->SetSpeed(MOVE_WALK, 0.5f, true);
                            me->GetMotionMaster()->MovePoint(POINT_GHOUL, centerPos);
                            //DoCast(me, SPELL_CALL_OF_THE_HIGHBORNE_2, true);
                            DoCast(me, SPELL_WRACKING_PAIN_AURA, true);
                            break;
                    }
                }
            }
        };
};

class spell_echo_of_sylvanas_wracking_pain_dmg : public SpellScriptLoader
{
    public:
        spell_echo_of_sylvanas_wracking_pain_dmg() : SpellScriptLoader("spell_echo_of_sylvanas_wracking_pain_dmg") { }

        class spell_echo_of_sylvanas_wracking_pain_dmg_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_echo_of_sylvanas_wracking_pain_dmg_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (!GetCaster() || !GetCaster()->isAlive())
                {
                    targets.clear();
                    return;
                }

                if (!GetCaster()->GetAI())
                {
                    targets.clear();
                    return;
                }

                uint64 _guid = GetCaster()->GetAI()->GetGUID(DATA_GUID);
                if (Creature* pTarget = ObjectAccessor::GetCreature(*GetCaster(), _guid))
                {
                    if (pTarget->isAlive())
                        targets.remove_if(WrackingPainTargetSelector(GetCaster(), pTarget));
                    else
                        targets.clear();
                }
                else
                    targets.clear();
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_echo_of_sylvanas_wracking_pain_dmg_SpellScript::FilterTargets, EFFECT_0,TARGET_UNIT_SRC_AREA_ENEMY);
            }

        private:

            class WrackingPainTargetSelector
            {
                public:
                    WrackingPainTargetSelector(WorldObject* caster, WorldObject* target) : i_caster(caster), i_target(target) { }

                    bool operator()(WorldObject* unit)
                    {
                        if (unit->IsInBetween(i_caster, i_target))
                            return false;
                        return true;
                    }

                    WorldObject* i_caster;
                    WorldObject* i_target;
            };
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_echo_of_sylvanas_wracking_pain_dmg_SpellScript();
        }
};

class spell_echo_of_sylvanas_death_grip_aoe : public SpellScriptLoader
{
    public:
        spell_echo_of_sylvanas_death_grip_aoe() : SpellScriptLoader("spell_echo_of_sylvanas_death_grip_aoe") { }

        class spell_echo_of_sylvanas_death_grip_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_echo_of_sylvanas_death_grip_aoe_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
			{
				if(!GetCaster() || !GetHitUnit())
					return;

                GetHitUnit()->CastSpell(GetCaster(), SPELL_DEATH_GRIP, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_echo_of_sylvanas_death_grip_aoe_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_echo_of_sylvanas_death_grip_aoe_SpellScript();
        }
};

class spell_echo_of_sylvanas_seeping_shadows : public SpellScriptLoader
{
    public:
        spell_echo_of_sylvanas_seeping_shadows() : SpellScriptLoader("spell_echo_of_sylvanas_seeping_shadows") { }

        class spell_echo_of_sylvanas_seeping_shadows_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_echo_of_sylvanas_seeping_shadows_AuraScript);

            void HandlePeriodicTick(constAuraEffectPtr /*aurEff*/)
            {
                if (!GetCaster())
                    return;

                int32 amount = int32(0.2f * (100.0f - GetCaster()->GetHealthPct()));

                if (AuraPtr aur = GetCaster()->GetAura(103182))
                    aur->ModStackAmount(amount - aur->GetStackAmount());
                else
                    GetCaster()->CastCustomSpell(103182, SPELLVALUE_AURA_STACK, amount, GetCaster(), true);

            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_echo_of_sylvanas_seeping_shadows_AuraScript::HandlePeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_echo_of_sylvanas_seeping_shadows_AuraScript();
        }
};

class achievement_several_ties : public AchievementCriteriaScript
{
    public:
        achievement_several_ties() : AchievementCriteriaScript("achievement_several_ties") { }

        bool OnCheck(Player* source, Unit* target)
        {
            if (!target)
                return false;

            if (boss_echo_of_sylvanas::boss_echo_of_sylvanasAI* echo_of_sylvanasAI = CAST_AI(boss_echo_of_sylvanas::boss_echo_of_sylvanasAI, target->GetAI()))
                return echo_of_sylvanasAI->AllowAchieve();

            return false;
        }
};

void AddSC_boss_echo_of_sylvanas()
{
    new boss_echo_of_sylvanas();
    new npc_echo_of_sylvanas_ghoul();
    new npc_echo_of_sylvanas_risen_ghoul();
    new spell_echo_of_sylvanas_wracking_pain_dmg();
    new spell_echo_of_sylvanas_death_grip_aoe();
    new spell_echo_of_sylvanas_seeping_shadows();
    new achievement_several_ties();
}
