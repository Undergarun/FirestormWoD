#include "ScriptPCH.h"
#include "end_time.h"

enum Yells
{
    SAY_AGGRO   = 0,
    SAY_DEATH   = 1,
    SAY_INTRO   = 2,
    SAY_KILL    = 3,
    SAY_SPELL   = 4,
};

enum Spells
{
    SPELL_BAINE_VISUALS     = 101624,

    SPELL_THROW_TOTEM       = 101615,
    SPELL_PULVERIZE         = 101626, 
    SPELL_PULVERIZE_DMG     = 101627, 
    SPELL_PULVERIZE_AOE     = 101625,
    SPELL_MOLTEN_AXE        = 101836,
    SPELL_MOLTEN_FIST       = 101866,
    SPELL_THROW_TOTEM_BACK  = 101602,
    SPELL_THROW_TOTEM_AURA  = 107837,
};

enum Events
{
    EVENT_PULVERIZE     = 1,
    EVENT_THROW_TOTEM   = 2,
    EVENT_CHECK_SELF    = 3,
};

enum Adds
{
    NPC_ROCK_ISLAND     = 54496,
    NPC_BAINES_TOTEM    = 54434,
};

class boss_echo_of_baine : public CreatureScript
{
    public:
        boss_echo_of_baine() : CreatureScript("boss_echo_of_baine") { }

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_echo_of_baineAI(pCreature);
        }

        struct boss_echo_of_baineAI : public BossAI
        {
            boss_echo_of_baineAI(Creature* pCreature) : BossAI(pCreature, DATA_ECHO_OF_BAINE)
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
                bIntroDone = false;
            }

            bool bIntroDone;

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
                me->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 5.0f);
                me->SetFloatValue(UNIT_FIELD_COMBATREACH, 5.0f);
            }

            void EnterCombat(Unit* /*who*/)
            {
                Talk(SAY_AGGRO);

                events.ScheduleEvent(EVENT_PULVERIZE, 60000);
                events.ScheduleEvent(EVENT_THROW_TOTEM, 25000);
                events.ScheduleEvent(EVENT_THROW_TOTEM, 25000);
                events.ScheduleEvent(EVENT_CHECK_SELF, 1000);

                instance->SetBossState(DATA_ECHO_OF_BAINE, IN_PROGRESS);
                DoZoneInCombat();
            }

            void EnterEvadeMode()
            {
                instance->SetData(DATA_PLATFORMS, NOT_STARTED);
                BossAI::EnterEvadeMode();
            }

            void MoveInLineOfSight(Unit* who)
            {
                if (bIntroDone)
                    return;

                if (who->GetTypeId() != TYPEID_PLAYER)
                    return;

                if (!me->IsWithinDistInMap(who, 100.0f, false))
                    return;

                Talk(SAY_INTRO);
                bIntroDone = true;
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
                                DoCast(pPlayer, SPELL_ARCHIVED_BAINE, true);
            }

            void KilledUnit(Unit * /*victim*/)
            {
                Talk(SAY_KILL);
            }

            void MovementInform(uint32 type, uint32 data)
            {
                if (type == EFFECT_MOTION_TYPE)
                    if (data == EVENT_JUMP)
                        DoCastAOE(SPELL_PULVERIZE_DMG);
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
                        case EVENT_CHECK_SELF:
                            //if (me->HasUnitMovementFlag(MOVEMENTFLAG_SWIMMING) && !me->HasAura(SPELL_MOLTEN_AXE))
                                //DoCast(me, SPELL_MOLTEN_AXE);
                            if (me->GetPositionY() < 1398.0f)
                            {
                                EnterEvadeMode();
                                return;
                            }
                            events.ScheduleEvent(EVENT_CHECK_SELF, 1000);
                            break;
                        case EVENT_PULVERIZE:
                            Talk(SAY_SPELL);
                            DoCastAOE(SPELL_PULVERIZE_AOE);
                            events.ScheduleEvent(EVENT_PULVERIZE, 60000);
                            events.ScheduleEvent(EVENT_THROW_TOTEM, 25000);
                            events.ScheduleEvent(EVENT_THROW_TOTEM, 25000);
                            break;
                        case EVENT_THROW_TOTEM:
                        {
                            Unit* pTarget = NULL;
                            pTarget = SelectTarget(SELECT_TARGET_FARTHEST, 1, PositionSelector());
                            if (!pTarget)
                                pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, PositionSelector());
                            if (pTarget)
                                DoCast(pTarget, SPELL_THROW_TOTEM);
                            break;
                        }
                    }
                }

                DoMeleeAttackIfReady();
            }
        private:
            struct PositionSelector : public std::unary_function<Unit*, bool>
            {
                public:
                    
                    //PositionSelector(bool b) : _b(b) {}

                    bool operator()(Unit const* target) const
                    {
                        if (target->GetTypeId() != TYPEID_PLAYER)
                            return false;

                        if (target->GetAreaId() != AREA_OBSIDIAN)
                            return false;

                        if (target->IsInWater())
                            return false;

                        return true;
                    }
                private:
                    bool _b;
            };
        };      
};

class npc_echo_of_baine_baines_totem : public CreatureScript
{
    public:
        npc_echo_of_baine_baines_totem() : CreatureScript("npc_echo_of_baine_baines_totem") { }

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_echo_of_baine_baines_totemAI(pCreature);
        }

        struct npc_echo_of_baine_baines_totemAI : public Scripted_NoMovementAI
        {
            npc_echo_of_baine_baines_totemAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
            {
                me->SetReactState(REACT_PASSIVE);
                bDespawn = false;
            }

            bool bDespawn;

            void OnSpellClick(Unit* /*who*/)
            {
                if (!bDespawn)
                {
                    bDespawn = true;
                    me->DespawnOrUnsummon();
                }
            }
        };      
};

class spell_echo_of_baine_pulverize_aoe : public SpellScriptLoader
{
    public:
        spell_echo_of_baine_pulverize_aoe() : SpellScriptLoader("spell_echo_of_baine_pulverize_aoe") { }

        class spell_echo_of_baine_pulverize_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_echo_of_baine_pulverize_aoe_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (!GetCaster() || !GetCaster()->getVictim())
                    return;

                std::list<WorldObject*> tempList;
                for (std::list<WorldObject*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
                    if ((*itr)->ToUnit() && !(*itr)->ToUnit()->IsInWater())
                        tempList.push_back((*itr));

                if (tempList.size() > 1)
                    tempList.remove(GetCaster()->getVictim());

                targets.clear();
                if (!tempList.empty())
                    targets.push_back(JadeCore::Containers::SelectRandomContainerElement(tempList));
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
			{
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetHitUnit(), SPELL_PULVERIZE, true);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_echo_of_baine_pulverize_aoe_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnEffectHitTarget += SpellEffectFn(spell_echo_of_baine_pulverize_aoe_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_echo_of_baine_pulverize_aoe_SpellScript();
        }
};

void AddSC_boss_echo_of_baine()
{
    new boss_echo_of_baine();
    new npc_echo_of_baine_baines_totem();
    new spell_echo_of_baine_pulverize_aoe();
}