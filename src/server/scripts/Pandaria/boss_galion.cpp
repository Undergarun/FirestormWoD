////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ObjectMgr.h"
#include "SpellScript.h"
#include "ScriptMgr.h"
#include "Vehicle.h"

enum eBosses
{
    BOSS_GALION
};

enum eSpells
{
    SPELL_FIRE_SHOT     = 121673,
    SPELL_STOMP         = 121787,

    SPELL_CANON_1       = 121577,
    SPELL_CANON_2       = 121605,
    SPELL_CANON_3       = 121607,
    SPELL_CANON_4       = 121608,
    SPELL_CANON_5       = 121609,
    SPELL_CANON_6       = 121610,
    SPELL_CANON_7       = 121611,
    SPELL_CANON_8       = 121613,
    SPELL_CANON_DMG     = 121600,

    SPELL_IMPALING_PULL = 121747,
    SPELL_GRIP_TARGET   = 75686,

    SPELL_GALLEON_BONUS = 132206
};

uint32 const CanonSpells[8] = { SPELL_CANON_1, SPELL_CANON_2, SPELL_CANON_3, SPELL_CANON_4, SPELL_CANON_5, SPELL_CANON_6, SPELL_CANON_7, SPELL_CANON_8 };

enum eEvents
{
    EVENT_FIRE_SHOT = 1,
    EVENT_WARN_STOMP,
    EVENT_STOMP,
    EVENT_CANON_BARRAGE,
    EVENT_WARMONGER,
    EVENT_IMPALING_PULL
};

enum eCreatures
{
    NPC_GALLEON             = 62351,
    NPC_GALLEON_CANON       = 62355,
    NPC_SALYIN_WARMONGER    = 62351,
    NPC_CHIEF_SALYIS        = 62352     // Seat ID: 2
};

enum eSays
{
    TALK_SPAWN,     // Chief Salyis yells: Loot and pillage, bwahahaha!
    TALK_AGGRO,     // Chief Salyis yells: Bring me their corpses!
    TALK_CANON,     // Chief Salyis yells: Arm the cannons! Ready! Aim! FIRE!
    TALK_SLAY,      // Chief Salyis yells: They are soft, weak!
    TALK_DEATH      // Chief Salyis yells: Retreat to the hills!
};

enum eActions
{
    ACTION_TALK_INTRO
};

// Galleon - 62346
class boss_galion : public CreatureScript
{
    public:
        boss_galion() : CreatureScript("boss_galion") { }

        struct boss_galion_AI : public BossAI
        {
            boss_galion_AI(Creature* p_Creature) : BossAI(p_Creature, BOSS_GALION)
            {
                ASSERT(me->GetVehicleKit());
            }

            EventMap m_Events;
            uint32 m_CheckVehicleTimer;
            uint8 m_CanonReady;
            std::list<uint64> m_LootersGuids;

            void CheckVehicleAccessories()
            {
                Unit* l_ChiefSalyis = me->GetVehicleKit()->GetPassenger(2);
                Position l_Pos;
                me->GetPosition(&l_Pos);

                if (!l_ChiefSalyis)
                    l_ChiefSalyis = me->SummonCreature(NPC_CHIEF_SALYIS, l_Pos);

                l_ChiefSalyis->EnterVehicle(me, 2);
                l_ChiefSalyis->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                l_ChiefSalyis->ToCreature()->AI()->DoAction(ACTION_TALK_INTRO);

                std::list<Creature*> l_Canons;
                me->GetCreatureListWithEntryInGrid(l_Canons, NPC_GALLEON_CANON, 100.0f);
                if (l_Canons.size() != 2)
                {
                    for (Creature* l_Canon : l_Canons)
                        summons.Despawn(l_Canon);

                    for (uint8 l_Index = 0; l_Index < 2; ++l_Index)
                    {
                        if (Creature* l_NewCanon = me->SummonCreature(NPC_GALLEON_CANON, l_Pos))
                        {
                            l_NewCanon->EnterVehicle(me, l_Index);
                            l_NewCanon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                            l_NewCanon->SetReactState(REACT_PASSIVE);
                        }
                    }
                }

                std::list<Creature*> l_Warmongers;
                me->GetCreatureListWithEntryInGrid(l_Warmongers, NPC_SALYIN_WARMONGER, 200.0f);
                if (l_Warmongers.size() != 2)
                {
                    for (Creature* l_Warmonger : l_Warmongers)
                        summons.Despawn(l_Warmonger);

                    for (uint8 l_Index = 3; l_Index < 7; ++l_Index)
                    {
                        if (Creature* l_Warmonger = me->SummonCreature(NPC_SALYIN_WARMONGER, l_Pos))
                        {
                            l_Warmonger->EnterVehicle(me, l_Index);
                            l_Warmonger->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        }
                    }
                }
            }

            void Reset()
            {
                m_Events.Reset();

                _Reset();

                summons.DespawnAll();

                m_CheckVehicleTimer = 2000;
                m_CanonReady        = 0;

                m_LootersGuids.clear();
            }

            void JustSummoned(Creature* p_Summon)
            {
                summons.Summon(p_Summon);
                p_Summon->setActive(true);
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                if (Unit* l_ChiefSalyis = me->GetVehicleKit()->GetPassenger(2))
                    l_ChiefSalyis->ToCreature()->AI()->Talk(TALK_DEATH);

                summons.DespawnAll();

                for (uint64 l_Guid : m_LootersGuids)
                {
                    if (Player* l_Player = Player::GetPlayer(*me, l_Guid))
                        me->CastSpell(l_Player, SPELL_GALLEON_BONUS, true);
                }
            }

            void KilledUnit(Unit* p_Killer)
            {
                if (p_Killer->IsPlayer())
                {
                    if (Unit* l_ChiefSalyis = me->GetVehicleKit()->GetPassenger(2))
                        l_ChiefSalyis->ToCreature()->AI()->Talk(TALK_SLAY);
                }
            }

            void EnterCombat(Unit* p_Attacker)
            {
                m_Events.ScheduleEvent(EVENT_WARN_STOMP, 47000);
                m_Events.ScheduleEvent(EVENT_STOMP, 50000);
                m_Events.ScheduleEvent(EVENT_CANON_BARRAGE, 24000);

                if (Unit* l_ChiefSalyis = me->GetVehicleKit()->GetPassenger(2))
                    l_ChiefSalyis->ToCreature()->AI()->Talk(TALK_AGGRO);

                for (uint8 l_Index = 3; l_Index < 7; ++l_Index)
                {
                    if (Unit* l_Warmonger = me->GetVehicleKit()->GetPassenger(l_Index))
                        l_Warmonger->ToCreature()->AI()->AttackStart(p_Attacker);
                }
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& p_Damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (p_Damage >= me->GetHealth())
                {
                    std::list<HostileReference*> l_ThreatList = me->getThreatManager().getThreatList();
                    for (std::list<HostileReference*>::const_iterator l_Itr = l_ThreatList.begin(); l_Itr != l_ThreatList.end(); ++l_Itr)
                    {
                        if (Player* l_Player = Player::GetPlayer(*me, (*l_Itr)->getUnitGuid()))
                            m_LootersGuids.push_back(l_Player->GetGUID());
                    }
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (m_CheckVehicleTimer)
                {
                    if (m_CheckVehicleTimer <= p_Diff)
                    {
                        CheckVehicleAccessories();
                        m_CheckVehicleTimer = 0;
                    }
                    else
                        m_CheckVehicleTimer -= p_Diff;
                }

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_WARN_STOMP:
                        me->MonsterTextEmote("Galleon is about to |cFFFF0000|Hspell:121787|h[Stomp]|h|r !", 0, true);
                        break;
                    case EVENT_STOMP:
                        me->CastSpell(me, SPELL_STOMP, false);
                        m_Events.ScheduleEvent(EVENT_WARN_STOMP, 57000);
                        m_Events.ScheduleEvent(EVENT_STOMP, 60000);
                        m_Events.ScheduleEvent(EVENT_WARMONGER, 10000);
                        break;
                    case EVENT_CANON_BARRAGE:
                    {
                        if (!m_CanonReady)
                        {
                            me->MonsterTextEmote("Galleon prepares to unleash |cFFFF0000|Hspell:121600|h[Canon Barrage]|h|r !", 0, true);

                            if (Unit* l_ChiefSalyis = me->GetVehicleKit()->GetPassenger(2))
                                l_ChiefSalyis->ToCreature()->AI()->Talk(TALK_CANON);
                        }

                        if (m_CanonReady % 2)
                        {
                            if (Unit* l_Canon = me->GetVehicleKit()->GetPassenger(0))
                                l_Canon->CastSpell(l_Canon, CanonSpells[m_CanonReady], true);
                        }
                        else
                        {
                            if (Unit* l_Canon = me->GetVehicleKit()->GetPassenger(1))
                                l_Canon->CastSpell(l_Canon, CanonSpells[m_CanonReady], true);
                        }

                        ++m_CanonReady;

                        if (m_CanonReady < 8)
                            m_Events.ScheduleEvent(EVENT_CANON_BARRAGE, 1000);
                        else
                        {
                            m_CanonReady = 0;
                            m_Events.ScheduleEvent(EVENT_CANON_BARRAGE, 52000);
                        }

                        break;
                    }
                    case EVENT_WARMONGER:
                    {
                        Position l_Pos;
                        me->GetPosition(&l_Pos);

                        for (uint8 l_Index = 0; l_Index < 6; ++l_Index)
                            me->SummonCreature(NPC_SALYIN_WARMONGER, l_Pos);

                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new boss_galion_AI(p_Creature);
        }
};

// Chief Salyis - 62352
class npc_chief_salyis : public CreatureScript
{
    public:
        npc_chief_salyis() : CreatureScript("npc_chief_salyis") { }

        struct npc_chief_salyisAI : public ScriptedAI
        {
            npc_chief_salyisAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            bool m_IntroDone;

            void IsSummonedBy(Unit* /*p_Summoner*/)
            {
                m_IntroDone = false;
            }

            void DoAction(int32 const p_Action)
            {
                if (p_Action == ACTION_TALK_INTRO && !m_IntroDone)
                {
                    m_IntroDone = true;
                    Talk(TALK_SPAWN);
                }
            }

            void UpdateAI(const uint32 /*p_Diff*/) { }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_chief_salyisAI(p_Creature);
        }
};

// Salyin Warmonger - 62351
class npc_salyin_warmonger : public CreatureScript
{
    public:
        npc_salyin_warmonger() : CreatureScript("npc_salyin_warmonger") { }

        struct npc_salyin_warmongerAI : public ScriptedAI
        {
            npc_salyin_warmongerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;
            bool m_IsInCombat;

            void Reset()
            {
                m_Events.Reset();

                m_IsInCombat = false;
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                if (me->IsOnVehicle())
                    m_Events.ScheduleEvent(EVENT_FIRE_SHOT, urand(5000, 10000));
                else
                    m_Events.ScheduleEvent(EVENT_IMPALING_PULL, 20000);

                m_IsInCombat = true;
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!m_IsInCombat)
                    return;

                if (!UpdateVictim() && !me->IsOnVehicle())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING) && !me->IsOnVehicle())
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_FIRE_SHOT:
                        me->CastSpell(me, SPELL_FIRE_SHOT, false);
                        m_Events.ScheduleEvent(EVENT_FIRE_SHOT, urand(5000, 10000));
                        break;
                    case EVENT_IMPALING_PULL:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, SPELL_IMPALING_PULL, false);
                        m_Events.ScheduleEvent(EVENT_IMPALING_PULL, 20000);
                        break;
                    default:
                        break;
                }

                if (!me->IsOnVehicle())
                    DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_salyin_warmongerAI(p_Creature);
        }
};

// Impaling Pull - 121754
class spell_impaling_pull: public SpellScriptLoader
{
    public:
        spell_impaling_pull() : SpellScriptLoader("spell_impaling_pull") { }

        class spell_impaling_pull_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_impaling_pull_SpellScript);

            void HandleDummy(SpellEffIndex /*p_EffIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetHitUnit()->CastSpell(GetCaster(), SPELL_GRIP_TARGET, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_impaling_pull_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_impaling_pull_SpellScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_galion()
{
    new boss_galion();
    new npc_salyin_warmonger();
    new spell_impaling_pull();
}
#endif