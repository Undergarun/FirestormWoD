////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "upper_blackrock_spire.hpp"

enum eSpells
{
    SPELL_DEBILITATING_FIXATION_SEARCHER    = 177660,
    SPELL_DEBILITATING_FIXATION             = 161199,
    SPELL_REJUVENATING_SERUM                = 161203,
    SPELL_VILEBLOOD_SERUM_SEARCHER          = 161235,
    SPELL_VILEBLOOD_SERUM_ACTIVATED         = 161233,
    SPELL_VILEBLOOD_SERUM_MISSILE           = 161209,

    ///< Heroic only
    SPELL_SALVE_OF_TOXIC_FUMES_SEARCHER     = 162589,
    SPELL_SALVE_OF_TOXIC_FUMES_AURA         = 162600
};

enum eEvents
{
    EVENT_DEBILITATING_FIXATION = 1,
    EVENT_REJUVENATING_SERUM,
    EVENT_VILEBLOOD_SERUM,

    ///< Heroic only
    EVENT_SALVE_OF_TOXIC_FUMES,
    EVENT_ACTIVE_VILEBLOOD_SERUM
};

enum eSays
{
    TALK_AGGRO,
    TALK_DEATH,
    TALK_SLAY,
    TALK_SPELL_1,
    TALK_SPELL_2,
    TALK_SPELL_3,
    TALK_SPELL_4
};

// Kyrak The Corruptor - 76021
class boss_kyrak_the_corruptor : public CreatureScript
{
    public:
        boss_kyrak_the_corruptor() : CreatureScript("boss_kyrak_the_corruptor") { }

        struct boss_kyrak_the_corruptorAI : public BossAI
        {
            boss_kyrak_the_corruptorAI(Creature* p_Creature) : BossAI(p_Creature, DATA_KYRAK_THE_CORRUPTOR)
            {
                m_Instance = p_Creature->GetInstanceScript();
                m_InitializeAdds = 2000;
                m_DrakonidMonstrosityGuids.resize(2);
            }

            std::vector<uint64> m_DrakonidMonstrosityGuids;
            uint32 m_InitializeAdds;
            InstanceScript* m_Instance;
            EventMap m_Events;

            void Reset()
            {
                _Reset();

                me->ReenableEvadeMode();
                me->RemoveAllAreasTrigger();

                ResetAdds();

                m_Events.Reset();
            }

            void KilledUnit(Unit* p_Who)
            {
                if (p_Who->IsPlayer())
                    Talk(TALK_SLAY);
            }

            void EnterCombat(Unit* p_Attacker)
            {
                _EnterCombat();

                Talk(TALK_AGGRO);

                if (m_Instance)
                    m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                m_Events.ScheduleEvent(EVENT_DEBILITATING_FIXATION, 8000);
                m_Events.ScheduleEvent(EVENT_REJUVENATING_SERUM, 26000);

                if (IsHeroic())
                {
                    m_Events.ScheduleEvent(EVENT_SALVE_OF_TOXIC_FUMES, 12000);
                    m_Events.ScheduleEvent(EVENT_ACTIVE_VILEBLOOD_SERUM, 10000);
                }
            }

            void JustReachedHome()
            {
                if (m_Instance)
                    m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void JustDied(Unit* p_Killer)
            {
                _JustDied();

                Talk(TALK_DEATH);

                if (m_Instance)
                    m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                me->RemoveAllAreasTrigger();
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo)
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case SPELL_DEBILITATING_FIXATION_SEARCHER:
                        me->CastSpell(p_Target, SPELL_DEBILITATING_FIXATION, false);
                        Talk(urand(TALK_SPELL_1, TALK_SPELL_3));
                        break;
                    case SPELL_SALVE_OF_TOXIC_FUMES_SEARCHER:
                        me->CastSpell(p_Target, SPELL_SALVE_OF_TOXIC_FUMES_AURA, true);
                        break;
                    default:
                        break;
                }
            }

            void SpellHit(Unit* p_Caster, SpellInfo const* p_SpellInfo)
            {
                if (p_SpellInfo->Id == SPELL_VILEBLOOD_SERUM_ACTIVATED)
                {
                    m_Events.ScheduleEvent(EVENT_VILEBLOOD_SERUM, 5000);
                    Talk(TALK_SPELL_4);
                }
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo)
            {
                if (!me->HasAura(SPELL_VILEBLOOD_SERUM_ACTIVATED) && me->HealthBelowPctDamaged(50, p_Damage))
                    me->CastSpell(me, SPELL_VILEBLOOD_SERUM_SEARCHER, true);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                InitializeAdds(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_DEBILITATING_FIXATION:
                        me->CastSpell(me, SPELL_DEBILITATING_FIXATION_SEARCHER, true);
                        m_Events.ScheduleEvent(EVENT_DEBILITATING_FIXATION, 20000);
                        break;
                    case EVENT_REJUVENATING_SERUM:
                        me->CastSpell(me, SPELL_REJUVENATING_SERUM, false);
                        m_Events.ScheduleEvent(EVENT_REJUVENATING_SERUM, 33000);
                        break;
                    case EVENT_VILEBLOOD_SERUM:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100.f, true))
                        {
                            std::list<Unit*> l_PlayerList;
                            l_Target->GetPartyMembers(l_PlayerList);

                            for (Unit* l_Member : l_PlayerList)
                            {
                                if (l_Member->GetTypeId() != TYPEID_PLAYER)
                                    continue;

                                l_Target->CastSpell(l_Member, SPELL_VILEBLOOD_SERUM_MISSILE, true, NULL, nullptr, me->GetGUID());
                            }

                            l_Target->CastSpell(l_Target, SPELL_VILEBLOOD_SERUM_MISSILE, true, NULL, nullptr, me->GetGUID());
                        }
                        m_Events.ScheduleEvent(EVENT_VILEBLOOD_SERUM, 15000);
                        break;
                    case EVENT_SALVE_OF_TOXIC_FUMES:
                        me->CastSpell(me, SPELL_SALVE_OF_TOXIC_FUMES_SEARCHER, true);
                        m_Events.ScheduleEvent(EVENT_SALVE_OF_TOXIC_FUMES, 12000);
                        break;
                    case EVENT_ACTIVE_VILEBLOOD_SERUM:
                        me->CastSpell(me, SPELL_VILEBLOOD_SERUM_SEARCHER, true);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void InitializeAdds(uint32 const p_Diff)
            {
                if (!m_InitializeAdds)
                    return;

                if (m_InitializeAdds <= p_Diff)
                {
                    m_InitializeAdds = 0;

                    std::list<Creature*> l_DrakonidList;
                    me->GetCreatureListWithEntryInGrid(l_DrakonidList, NPC_DRAKONID_MONSTROSITY_2, 10.f);

                    // Should be 2
                    if (l_DrakonidList.empty() || l_DrakonidList.size() != 2)
                        return;

                    for (Creature* l_Drakonid : l_DrakonidList)
                        m_DrakonidMonstrosityGuids.push_back(l_Drakonid->GetGUID());
                }
                else
                    m_InitializeAdds -= p_Diff;
            }

            void ResetAdds()
            {
                if (m_DrakonidMonstrosityGuids.empty())
                    return;

                for (uint64 l_Guid : m_DrakonidMonstrosityGuids)
                {
                    if (Creature* l_Drakonid = Creature::GetCreature(*me, l_Guid))
                    {
                        l_Drakonid->Respawn();
                        l_Drakonid->GetMotionMaster()->Clear();
                        l_Drakonid->GetMotionMaster()->MoveTargetedHome();
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new boss_kyrak_the_corruptorAI(p_Creature);
        }
};

// Vileblood Serum - 161235
class spell_vilebloom_serum: public SpellScriptLoader
{
    public:
        spell_vilebloom_serum() : SpellScriptLoader("spell_vilebloom_serum") { }

        class spell_vilebloom_serum_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_vilebloom_serum_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                p_Targets.remove_if([this, l_Caster](WorldObject* p_Object) -> bool
                {
                    if (!p_Object)
                        return true;

                    if (p_Object->GetEntry() != NPC_KYRAK_THE_CORRUPTOR)
                        return true;

                    return false;
                });
            }

            void HandleDummy(SpellEffIndex p_EffIndex)
            {
                if (Unit* l_Target = GetHitUnit())
                {
                    if (Unit* l_Caster = GetCaster())
                        l_Caster->CastSpell(l_Target, SPELL_VILEBLOOD_SERUM_ACTIVATED, true);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_vilebloom_serum_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                OnEffectHitTarget += SpellEffectFn(spell_vilebloom_serum_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_vilebloom_serum_SpellScript();
        }
};

///< Vileblood Serum - 161210
class areatrigger_vileblood_serum : public AreaTriggerEntityScript
{
    public:
        areatrigger_vileblood_serum() : AreaTriggerEntityScript("areatrigger_vileblood_serum") { }

        enum eSpells
        {
            VilebloodSerum = 161288
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            std::list<Unit*> l_TargetList;
            float l_Radius = 2.0f;

            JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, p_AreaTrigger->GetCaster(), l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

            for (Unit* l_Unit : l_TargetList)
            {
                if (l_Unit->GetDistance(p_AreaTrigger) <= l_Radius)
                    l_Unit->CastSpell(l_Unit, eSpells::VilebloodSerum, true);
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new areatrigger_vileblood_serum();
        }
};

void AddSC_boss_kyrak_the_corruptor()
{
    new boss_kyrak_the_corruptor();
    new spell_vilebloom_serum();
    new areatrigger_vileblood_serum();
}
