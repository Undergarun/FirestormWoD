#include "ScriptPCH.h"
#include "end_time.h"
#include "Group.h"

enum MurozondScriptTexts
{
    SAY_AGGRO   = 0,
    SAY_DEATH   = 1,
    SAY_GLASS_1 = 2,
    SAY_GLASS_2 = 3,
    SAY_GLASS_3 = 4,
    SAY_GLASS_4 = 5,
    SAY_GLASS_5 = 6,
    SAY_INTRO_1 = 7,
    SAY_INTRO_2 = 8,
    SAY_KILL    = 9,
};

enum NozdormuScriptTexts
{
    SAY_NOZDORMU_INTRO      = 0,
    SAY_NOZDORMU_OUTRO_1    = 1,
    SAY_NOZDORMU_OUTRO_2    = 2,
    SAY_NOZDORMU_OUTRO_3    = 3,
    SAY_NOZDORMU_OUTRO_4    = 4,
    SAY_NOZDORMU_OUTRO_5    = 5,
};

enum Spells
{
    SPELL_INFINITE_BREATH               = 102569,
    SPELL_TEMPORAL_BLAST                = 102381,
    SPELL_TAIL_SWEEP                    = 108589,
    SPELL_FADING                        = 107550,
    SPELL_DISTORTION_BOMB               = 102516,
    SPELL_DISTORTION_BOMB_DMG           = 101984,
    SPELL_SANDS_OF_THE_HOURGLASS        = 102668,
    SPELL_TEMPORAL_SNAPSHOT             = 101592,
    SPELL_REWIND_TIME                   = 101590,
    SPELL_BLESSING_OF_BRONZE_DRAGONS    = 102364,
    SPELL_KILL_MUROZOND                 = 110158,
};

enum Events
{
    EVENT_INFINITE_BREATH   = 1,
    EVENT_TEMPORAL_BLAST    = 2,
    EVENT_TAIL_SWEEP        = 3,
    EVENT_DESPAWN           = 4,
    EVENT_DISTORTION_BOMB   = 5,
    EVENT_CHECK_ADDS        = 6,
    EVENT_INTRO_1           = 7,
    EVENT_INTRO_2           = 8,
    EVENT_INTRO_3           = 9,
    EVENT_NOZDORMU_INTRO    = 10,
    EVENT_CONTINUE          = 11,
    EVENT_NOZDORMU_OUTRO_1  = 12,
    EVENT_NOZDORMU_OUTRO_2  = 13,
    EVENT_NOZDORMU_OUTRO_3  = 14,
    EVENT_NOZDORMU_OUTRO_4  = 15,
    EVENT_NOZDORMU_OUTRO_5  = 16,
};

enum Adds
{
    NPC_MIRROR  = 54435,
};

enum Other
{
    POINT_LAND          = 1,
    ACTION_HOURGLASS    = 2,
    TYPE_HOURGLASS      = 3,
    ACTION_NOZDORMU     = 4,
};

const Position landPos = {4169.71f, -433.40f, 120.0f, 2.59f};

class boss_murozond : public CreatureScript
{
    public:
        boss_murozond() : CreatureScript("boss_murozond") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_murozondAI(creature);
        }

        struct boss_murozondAI : public BossAI
        {
            boss_murozondAI(Creature* creature) : BossAI(creature, DATA_MUROZOND)
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
                bDead = false;
                phase = 0;
                hourglass = 5;
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
            }

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
                bDead = false;
                hourglass = 5;
                me->RemoveAllDynObjects();
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SANDS_OF_THE_HOURGLASS);
            }

            void JustDied(Unit* /*killer*/)
            {
                _JustDied();
            }

            uint32 GetData(uint32 type)
            {
                if (type == TYPE_HOURGLASS)
                    return hourglass;
                return 0;
            }

            void MoveInLineOfSight(Unit* who)
            {
                if (phase)
                    return;

                if (me->GetDistance2d(who) >= 60.0f)
                    return;

                phase = 1;
                events.ScheduleEvent(EVENT_INTRO_1, 2000);
                events.ScheduleEvent(EVENT_INTRO_2, 28000);
                events.ScheduleEvent(EVENT_INTRO_3, 48000);
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_HOURGLASS)
                {
                    if (hourglass == 0)
                        return;

                    ActivateMirrors();
                    me->RemoveAllDynObjects();
                    me->AttackStop();
                    me->SetReactState(REACT_PASSIVE);
                    hourglass--;
                    instance->DoSetAlternatePowerOnPlayers(hourglass);
                    events.ScheduleEvent(EVENT_CONTINUE, 5000);
                    events.RescheduleEvent(EVENT_INFINITE_BREATH, urand(14000, 24000));
                    events.RescheduleEvent(EVENT_TAIL_SWEEP, 14000);
                    events.RescheduleEvent(EVENT_TEMPORAL_BLAST, 9000);
                    events.RescheduleEvent(EVENT_DISTORTION_BOMB, urand(9000, 12000));
                }
            }

            void EnterCombat(Unit* /*who*/)
            {
                Talk(SAY_AGGRO);
                events.ScheduleEvent(EVENT_INFINITE_BREATH, urand(10000, 20000));
                events.ScheduleEvent(EVENT_TAIL_SWEEP, 10000);
                events.ScheduleEvent(EVENT_TEMPORAL_BLAST, 5000);
                events.ScheduleEvent(EVENT_NOZDORMU_INTRO, 5000);
                events.ScheduleEvent(EVENT_DISTORTION_BOMB, urand(5000, 8000));

                DoCastAOE(SPELL_TEMPORAL_SNAPSHOT);
                instance->DoCastSpellOnPlayers(SPELL_BLESSING_OF_BRONZE_DRAGONS);
                instance->DoCastSpellOnPlayers(SPELL_SANDS_OF_THE_HOURGLASS);
                instance->SetBossState(DATA_MUROZOND, IN_PROGRESS);
                me->RemoveAllDynObjects();
                DoZoneInCombat();
            }

            void KilledUnit(Unit* who)
            {
                if (who && who->GetTypeId() == TYPEID_PLAYER)
                    Talk(SAY_KILL);
            }

            void CompleteEncounter()
            {
                if (instance)
                {
                    // Achievement
                    instance->DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_KILL_MUROZOND, 0, 0, me); 
                    
                    instance->UpdateEncounterState(ENCOUNTER_CREDIT_CAST_SPELL, SPELL_KILL_MUROZOND, me); 
                
                    if (GameObject* pGo = ObjectAccessor::GetGameObject(*me, instance->GetData64(DATA_HOURGLASS)))
                        pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);

                    instance->DoKilledMonsterKredit(QUEST_MUROZOND, 54432, 0);
                }
            }

            void JustReachedHome()
            {
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SANDS_OF_THE_HOURGLASS);
                BossAI::JustReachedHome();
            }

            void DamageTaken(Unit* /*who*/, uint32 &damage)
            {
                if (damage >= me->GetHealth())
                    damage = 0;
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim() && phase != 1)
                    return;

                if (!bDead && me->HealthBelowPct(8))
                {
                    bDead = true;
                    me->InterruptNonMeleeSpells(false);
                    me->RemoveAllAuras();
                    DoCast(me, SPELL_FADING, true);
                    CompleteEncounter();
                    events.ScheduleEvent(EVENT_DESPAWN, 3000);
                    Talk(SAY_DEATH);
                    return;
                }

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_INTRO_1:
                            Talk(SAY_INTRO_1);
                            break;
                        case EVENT_INTRO_2:
                            Talk(SAY_INTRO_2);
                            break;
                        case EVENT_INTRO_3:
                            phase = 2;
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                            break;
                        case EVENT_NOZDORMU_INTRO:
                            if (Creature* pNozdormu = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_NOZDORMU)))
                                pNozdormu->AI()->Talk(SAY_NOZDORMU_INTRO);
                            break;
                        case EVENT_DISTORTION_BOMB:
                        {
                            Unit* pTarget;
                            pTarget = SelectTarget(SELECT_TARGET_RANDOM, 1, -20.0f, true);
                            if (!pTarget)
                                pTarget = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true);
                            if (!pTarget)
                                pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true);
                            if (pTarget)
                                DoCast(pTarget, SPELL_DISTORTION_BOMB);
                            events.ScheduleEvent(EVENT_DISTORTION_BOMB, urand(5000, 8000));
                            break;
                        }
                        case EVENT_INFINITE_BREATH:
                            DoCastVictim(SPELL_INFINITE_BREATH);
                            events.ScheduleEvent(EVENT_INFINITE_BREATH, 15000);
                            break;
                        case EVENT_TAIL_SWEEP:
                            DoCast(me, SPELL_TAIL_SWEEP);
                            events.ScheduleEvent(EVENT_TAIL_SWEEP, 10000);
                            break;
                        case EVENT_TEMPORAL_BLAST:
                            DoCastAOE(SPELL_TEMPORAL_BLAST);
                            events.ScheduleEvent(EVENT_TEMPORAL_BLAST, urand(14000, 18000));
                            break;
                        case EVENT_CONTINUE:
                            Talk(SAY_GLASS_5 - hourglass);
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->GetMotionMaster()->MoveChase(me->getVictim());
                            break;
                        case EVENT_DESPAWN:
                            me->RemoveAllDynObjects();
                            if (Creature* pNozdormu = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_NOZDORMU)))
                                pNozdormu->AI()->DoAction(ACTION_NOZDORMU);
                            instance->SetBossState(DATA_MUROZOND, DONE);
                            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SANDS_OF_THE_HOURGLASS);
                            me->DespawnOrUnsummon(3000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        private:
            uint8 phase;
            bool bDead;
            uint32 hourglass;

            void ActivateMirrors()
            {
                //std::list<Creature*> mirrorList;
                //me->GetCreatureListWithEntryInGrid(mirrorList, NPC_MIRROR, 500.0f);
                
                //if (mirrorList.empty())
                //    return;

                //for (std::list<Creature*>::const_iterator itr = mirrorList.begin(); itr != mirrorList.end(); ++itr)
                //{
                //    if (Creature* pMirror = (*itr)->ToCreature())
                //        if (pMirror->isAlive() && pMirror->IsInWorld())
                //            pMirror->AI()->DoAction(ACTION_HOURGLASS);
                //}
                Map::PlayerList const &PlayerList = instance->instance->GetPlayers();
                if (!PlayerList.isEmpty())
                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                        if (Player* pPlayer = i->getSource())
                        {
                            if (!pPlayer->IsInWorld())
                                continue;

                            if (!pPlayer->isAlive())
                                pPlayer->ResurrectPlayer(1.0f, false);

                            std::list<uint32> spell_list;
                            SpellCooldowns cd_list = pPlayer->GetSpellCooldowns();
                            if (!cd_list.empty())
                            {
                                for (SpellCooldowns::const_iterator itr = cd_list.begin(); itr != cd_list.end(); ++itr)
                                {
                                    SpellInfo const* entry = sSpellMgr->GetSpellInfo(itr->first);
                                    if (entry &&
                                        entry->RecoveryTime <= 10 * MINUTE * IN_MILLISECONDS &&
                                        entry->CategoryRecoveryTime <= 10 * MINUTE * IN_MILLISECONDS)
                                    {
                                        spell_list.push_back(itr->first);
                                    }
                                }
                            }

                            pPlayer->RemoveAura(SPELL_TEMPORAL_BLAST);

                            pPlayer->SetHealth(pPlayer->GetMaxHealth());
                            pPlayer->SetPower(pPlayer->getPowerType(), pPlayer->GetMaxPower(pPlayer->getPowerType()));

                            pPlayer->CastSpell(pPlayer, SPELL_BLESSING_OF_BRONZE_DRAGONS, true);
                        }
            }
        };   
};

class npc_murozond_mirror_image : public CreatureScript
{
    public:
        npc_murozond_mirror_image() : CreatureScript("npc_murozond_mirror_image") { }

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_murozond_mirror_imageAI(pCreature);
        }

        struct npc_murozond_mirror_imageAI : public Scripted_NoMovementAI
        {
            npc_murozond_mirror_imageAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature) 
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                me->SetReactState(REACT_PASSIVE);
                m_owner = NULL;
                pInstance = me->GetInstanceScript();
            }

            Player* m_owner;
            InstanceScript* pInstance;

            void Reset()
            {
                if (!me->HasAura(SPELL_FADING))
                    me->AddAura(SPELL_FADING, me);
            }

            void IsSummonedBy(Unit* owner)
            {
                if (owner && owner->isAlive() && owner->IsInWorld())
                    if (owner->GetTypeId() == TYPEID_PLAYER)
                        m_owner = owner->ToPlayer();
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_HOURGLASS)
                {
                    if (m_owner && m_owner->IsInWorld())
                    {
                        if (!m_owner->isAlive())
                            m_owner->ResurrectPlayer(1.0f, false);

                        std::list<uint32> spell_list;
                        SpellCooldowns cd_list = m_owner->ToPlayer()->GetSpellCooldowns();
                        if (!cd_list.empty())
                        {
                            for (SpellCooldowns::const_iterator itr = cd_list.begin(); itr != cd_list.end(); ++itr)
                            {
                                SpellInfo const* entry = sSpellMgr->GetSpellInfo(itr->first);
                                if (entry &&
                                    entry->RecoveryTime <= 10 * MINUTE * IN_MILLISECONDS &&
                                    entry->CategoryRecoveryTime <= 10 * MINUTE * IN_MILLISECONDS)
                                {
                                    spell_list.push_back(itr->first);
                                }
                            }
                        }

                        m_owner->RemoveAura(SPELL_TEMPORAL_BLAST);

                        m_owner->SetHealth(m_owner->GetMaxHealth());
                        m_owner->SetPower(m_owner->getPowerType(), m_owner->GetMaxPower(m_owner->getPowerType()));

                        m_owner->CastSpell(m_owner, SPELL_BLESSING_OF_BRONZE_DRAGONS, true);
                        m_owner->NearTeleportTo(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), true);
                    }
                }
            }

            void UpdateAI(uint32 const diff)
            {
                if (!pInstance || pInstance->GetBossState(DATA_MUROZOND) != IN_PROGRESS)
                {
                    me->DespawnOrUnsummon();
                    return;
                }
            }
        };
};

class npc_end_time_nozdormu : public CreatureScript
{
    public:

        npc_end_time_nozdormu() : CreatureScript("npc_end_time_nozdormu") { }

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_end_time_nozdormuAI(pCreature);
        }

        struct npc_end_time_nozdormuAI : public ScriptedAI
        {
            npc_end_time_nozdormuAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                me->SetVisible(false);
                me->setActive(true);
                bTalk = false;
            }

            void Reset()
            {
                events.Reset();
            }

            void DoAction(const int32 action)
            {
                if (!bTalk && action == ACTION_NOZDORMU)
                {
                    bTalk = true;
                    me->SetVisible(true);
                    events.ScheduleEvent(EVENT_NOZDORMU_OUTRO_1, 5000);
                    events.ScheduleEvent(EVENT_NOZDORMU_OUTRO_2, 20000);
                    events.ScheduleEvent(EVENT_NOZDORMU_OUTRO_3, 33000);
                    events.ScheduleEvent(EVENT_NOZDORMU_OUTRO_4, 41000);
                    events.ScheduleEvent(EVENT_NOZDORMU_OUTRO_5, 44000);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!bTalk)
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_NOZDORMU_OUTRO_1:
                            Talk(SAY_NOZDORMU_OUTRO_1);
                            break;
                        case EVENT_NOZDORMU_OUTRO_2:
                            Talk(SAY_NOZDORMU_OUTRO_2);
                            break;
                        case EVENT_NOZDORMU_OUTRO_3:
                            Talk(SAY_NOZDORMU_OUTRO_3);
                            break;
                        case EVENT_NOZDORMU_OUTRO_4:
                            if (GameObject* pGo = me->FindNearestGameObject(HOURGLASS_OF_TIME, 300.0f))
                                me->SetFacingToObject(pGo);
                            break;
                        case EVENT_NOZDORMU_OUTRO_5:
                            Talk(SAY_NOZDORMU_OUTRO_5);
                            bTalk = false;
                            break;
                    }
                }
            }

        private:
            bool bTalk;
            EventMap events;
        };
};

class go_murozond_hourglass_of_time : public GameObjectScript
{
    public:
        go_murozond_hourglass_of_time() : GameObjectScript("go_murozond_hourglass_of_time") { }

        bool OnGossipHello(Player* pPlayer, GameObject* pGo)
        {
            InstanceScript* pInstance = pGo->GetInstanceScript();
            if (!pInstance || pInstance->GetBossState(DATA_MUROZOND) != IN_PROGRESS)
                return true;

            if (Creature* pMurozond = ObjectAccessor::GetCreature(*pGo, pInstance->GetData64(DATA_MUROZOND)))
            {
                if (pMurozond->AI()->GetData(TYPE_HOURGLASS) == 0)
                    return true;

                pMurozond->AI()->DoAction(ACTION_HOURGLASS);
            }

            return false;
        }
};

void AddSC_boss_murozond()
{
    new boss_murozond();
    new npc_end_time_nozdormu();
    new npc_murozond_mirror_image();
    new go_murozond_hourglass_of_time();
}
