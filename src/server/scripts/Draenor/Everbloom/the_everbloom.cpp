////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "the_everbloom.hpp"

namespace
{
    void DespawnCreaturesInArea(uint32 entry, WorldObject* object)
    {
        std::list<Creature*> creatures;
        GetCreatureListWithEntryInGrid(creatures, object, entry, 10.0f);
        if (creatures.empty())
            return;

        for (std::list<Creature*>::iterator iter = creatures.begin(); iter != creatures.end(); ++iter)
            (*iter)->DespawnOrUnsummon();
    }
}

/// Dreadpital toxin - 81864
class the_everbloom_dreadpital_toxin : public CreatureScript
{
public:
    the_everbloom_dreadpital_toxin() : CreatureScript("the_everbloom_Dreadpital_Toxin") { }

    struct the_everbloom_creaturesAI : public ScriptedAI
    {
        the_everbloom_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();

            me->setFaction(16);
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eEverbloomEvents::EventDreadpetalToxin, urand(8 * TimeConstants::IN_MILLISECONDS, 14 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eEverbloomEvents::EventDreadpetalToxin:
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 20.0f, true))
                        me->CastSpell(l_Target, eEverbloomSpells::SpellDreadpetalToxin);

                    events.ScheduleEvent(eEverbloomEvents::EventDreadpetalToxin, urand(8 * TimeConstants::IN_MILLISECONDS, 14 * TimeConstants::IN_MILLISECONDS));
                    break;
            }

            DoMeleeAttackIfReady();

        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new the_everbloom_creaturesAI(pCreature);
    }
};

/// Everbloom Tender - 81985
class the_everbloom_tender : public CreatureScript
{
public:
    the_everbloom_tender() : CreatureScript("the_everbloom_tender") { }

    struct the_everbloom_creaturesAI : public ScriptedAI
    {
        the_everbloom_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();

            me->CastSpell(me, eEverbloomSpells::SpellSolarChannel);
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eEverbloomEvents::EventDancingThorns, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eEverbloomEvents::EventEnragedGrowth, urand(7 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case eEverbloomEvents::EventDancingThorns:
                if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                    me->CastSpell(l_Target, eEverbloomSpells::SpellDancingThorns);

                events.ScheduleEvent(eEverbloomEvents::EventDancingThorns, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
                break;
            case eEverbloomEvents::EventEnragedGrowth:
                if (Creature* l_Petal = me->FindNearestCreature(eEverbloomCreature::CreatureDreadpetalToxin, 20.0f, true))
                    me->CastSpell(l_Petal, eEverbloomSpells::SpellEnragedGrowth);

                events.ScheduleEvent(eEverbloomEvents::EventEnragedGrowth, urand(12 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));
                break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new the_everbloom_creaturesAI(pCreature);
    }
};

///  Melder Berserker - 86372
class the_everbloom_melded_berserker : public CreatureScript
{
public:
    the_everbloom_melded_berserker() : CreatureScript("the_everbloom_melded_berserker") { }

    struct the_everbloom_creaturesAI : public ScriptedAI
    {
        the_everbloom_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            me->CastSpell(me, eEverbloomSpells::SpellEnrage);

            events.ScheduleEvent(eEverbloomEvents::EventVileBreath, urand(10 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eEverbloomEvents::EventBoundingWhirl, 20 * TimeConstants::IN_MILLISECONDS);
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
            case eEverbloomActions::ActionBoundingWhirlAura:
                me->CastSpell(me, eEverbloomSpells::SpellBoundingWhirlAura);
                break;
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case eEverbloomEvents::EventVileBreath:
                if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                    me->CastSpell(l_Target, eEverbloomSpells::SpellVileBreath);

                events.ScheduleEvent(eEverbloomEvents::EventVileBreath, urand(12 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
                break;

            case eEverbloomEvents::EventBoundingWhirl:
                if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                {
                    me->CastSpell(me, eEverbloomSpells::SpellBoundingWhirlJump);
                }

                events.ScheduleEvent(eEverbloomEvents::EventBoundingWhirl, 20 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eEverbloomEvents::EventBoundingWhirl2, 2 * TimeConstants::IN_MILLISECONDS);
                break;
            case eEverbloomEvents::EventBoundingWhirl2:
                me->CastSpell(me, eEverbloomSpells::SpellBoundingWhirlAura);
                break;

            }

            DoMeleeAttackIfReady();

        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new the_everbloom_creaturesAI(pCreature);
    }
};

/// Everbloom Gnarloot - 81984
class the_everbloom_gnarlroot : public CreatureScript
{
public:
    the_everbloom_gnarlroot() : CreatureScript("the_everbloom_gnarlroot") { }

    struct the_everbloom_creaturesAI : public ScriptedAI
    {
        the_everbloom_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eEverbloomEvents::EventGasp, urand(10 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eEverbloomEvents::EventBarrageOfLeaves, 15 * TimeConstants::IN_MILLISECONDS);
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eEverbloomEvents::EventGasp:
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eEverbloomSpells::SpellGasp);

                    events.ScheduleEvent(eEverbloomEvents::EventGasp, urand(10 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
                    break;
                case eEverbloomEvents::EventBarrageOfLeaves:
                    me->CastSpell(me, eEverbloomSpells::SpellLivingLeavesDummy);

                    events.ScheduleEvent(eEverbloomEvents::EventBarrageOfLeaves, 30 * TimeConstants::IN_MILLISECONDS);
                    break;
            }

            DoMeleeAttackIfReady();
        }

    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new the_everbloom_creaturesAI(pCreature);
    }
};

/// Verdant Mandragora - 81983
class the_everbloom_verdant_mandragora : public CreatureScript
{
public:
    the_everbloom_verdant_mandragora() : CreatureScript("the_everbloom_verdant_mandragora") { }

    struct the_everbloom_creaturesAI : public ScriptedAI
    {
        the_everbloom_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
        }

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eEverbloomEvents::EventVirulendGasp, urand(8 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eEverbloomEvents::EventVirulendGasp:
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eEverbloomSpells::SpellVirulendGasp);

                    events.ScheduleEvent(eEverbloomEvents::EventVirulendGasp, urand(7 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
                    break;
            }

            DoMeleeAttackIfReady();
        }

    };
    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new the_everbloom_creaturesAI(pCreature);
    }
};

/// Everbloom Mender - 81820
class the_everbloom_mender : public CreatureScript
{
public:
    the_everbloom_mender() : CreatureScript("the_everbloom_mender") { }

    struct the_everbloom_creaturesAI : public ScriptedAI
    {
        the_everbloom_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
        }

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eEverbloomEvents::EventChokingVines, 18 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eEverbloomEvents::EventHealingWaters, 10 * TimeConstants::IN_MILLISECONDS);
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eEverbloomEvents::EventChokingVines:
                {
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                        me->CastSpell(l_Target, eEverbloomSpells::SpellChokingVines);

                    events.ScheduleEvent(eEverbloomEvents::EventChokingVines, 18 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eEverbloomEvents::EventHealingWaters:
                {
                    if (Unit* friendUnit = DoSelectLowestHpFriendly(85)) // heal
                        me->CastSpell(friendUnit, eEverbloomSpells::SpellHealingWaters);

                    events.ScheduleEvent(eEverbloomEvents::EventHealingWaters, 10 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new the_everbloom_creaturesAI(pCreature);
    }
};


/// Twisted Abomination - 84767
class the_everbloom_twisted_abomination : public CreatureScript
{
public:
    the_everbloom_twisted_abomination() : CreatureScript("the_everbloom_twisted_abomination") { }

    struct the_everbloom_creaturesAI : public ScriptedAI
    {
        the_everbloom_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
        }

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eEverbloomEvents::EventNoxiousEruption, 20 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eEverbloomEvents::EventPoisonousClaws, urand(8 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eEverbloomEvents::EventNoxiousEruption:
                {
                    me->CastSpell(me, eEverbloomSpells::SpellAbominationNoxiousErupt);
                    events.ScheduleEvent(eEverbloomEvents::EventNoxiousEruption, 20 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eEverbloomEvents::EventPoisonousClaws:
                {
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eEverbloomSpells::SpellPoisonousClaws);

                    events.ScheduleEvent(eEverbloomEvents::EventPoisonousClaws, urand(8 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
                    break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new the_everbloom_creaturesAI(pCreature);
    }
};

/// Infested Icecaller - 84989
class the_everbloom_infested_icecaller : public CreatureScript
{
public:
    the_everbloom_infested_icecaller() : CreatureScript("the_everbloom_infested_icecaller") { }

    struct the_everbloom_creaturesAI : public ScriptedAI
    {
        the_everbloom_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
        }

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eEverbloomEvents::EventFrozenSnap, urand(10 * TimeConstants::IN_MILLISECONDS, 17 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eEverbloomEvents::EventFrostbolt, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eEverbloomEvents::EventFrostbolt:
                    if (Unit* l_Target = me->getVictim())
                    {
                        me->CastSpell(l_Target, eEverbloomSpells::SpellFrostbolt);

                        events.ScheduleEvent(eEverbloomEvents::EventFrostbolt, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
                    }
                    break;
                case eEverbloomEvents::EventFrozenSnap:
                    for (int i = 0; i <= 3; i++)
                    {
                        events.CancelEvent(eEverbloomEvents::EventFrozenSnap);

                        Position l_Pos;
                        me->GetRandomNearPosition(l_Pos, 20.0f);

                        me->SummonCreature(eEverbloomCreature::TriggerFrozenSnap, l_Pos, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 4 * TimeConstants::IN_MILLISECONDS);
                        events.ScheduleEvent(eEverbloomEvents::EventFrozenSnap, urand(10 * TimeConstants::IN_MILLISECONDS, 13 * TimeConstants::IN_MILLISECONDS));
                    }
                    break;
            }

            DoMeleeAttackIfReady();
        }

    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new the_everbloom_creaturesAI(pCreature);
    }
};

///  Addled Arcanomancer - 84990
class the_everbloom_addled_arcanomancer : public CreatureScript
{
public:
    the_everbloom_addled_arcanomancer() : CreatureScript("the_everbloom_addled_arcanomancer") { }

    struct the_everbloom_creaturesAI : public ScriptedAI
    {
        the_everbloom_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
        }

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eEverbloomEvents::EventArcaneBlast, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eEverbloomEvents::EventArcaneBlast:
                    if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0, true))
                    {
                        me->CastSpell(l_Random, eEverbloomSpells::SpellArcaneBlast);
                        events.ScheduleEvent(eEverbloomEvents::EventArcaneBlast, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
                    }
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new the_everbloom_creaturesAI(pCreature);
    }
};

/// Putrid Pyromancer - 84957
class the_everbloom_putrid_pyromancer : public CreatureScript
{
public:
    the_everbloom_putrid_pyromancer() : CreatureScript("the_everbloom_putrid_pyromancer") { }

    struct the_everbloom_creaturesAI : public ScriptedAI
    {
        the_everbloom_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
        }

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eEverbloomEvents::EventDragonsBreath, urand(14 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eEverbloomEvents::EventFireball, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eEverbloomEvents::EventDragonsBreath:
                    if (Unit* l_Random = me->getVictim())
                    {
                        me->CastSpell(l_Random, eEverbloomSpells::SpellDragonsBreath);
                        events.ScheduleEvent(eEverbloomEvents::EventDragonsBreath, urand(40 * TimeConstants::IN_MILLISECONDS, 50 * TimeConstants::IN_MILLISECONDS));
                    }
                    break;
                case eEverbloomEvents::EventFireball:
                    me->CastSpell(me, eEverbloomSpells::SpellFireBall);
                    events.ScheduleEvent(eEverbloomEvents::EventFireball, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new the_everbloom_creaturesAI(pCreature);
    }
};

// Spore Breath
class the_everbloom_spore_breath_infect : public SpellScriptLoader
{
public:
    the_everbloom_spore_breath_infect() : SpellScriptLoader("the_everbloom_spore_breath_infect") { }

    class the_the_everbloom_spells : public SpellScript
    {
        PrepareSpellScript(the_the_everbloom_spells);

        void SporeBreathInfectBarrage()
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (Unit* l_Target = GetExplTargetUnit())
                {
                    for (int i = 0; i <= 12; i++)
                        l_Caster->CastSpell(l_Target, eEverbloomSpells::SpellSporeBreathInfect, true);
                }
            }
        }

        void Register()
        {
            AfterCast += SpellCastFn(the_the_everbloom_spells::SporeBreathInfectBarrage);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new the_the_everbloom_spells();
    }
};

// Venom Spray
class the_everbloom_venom_spray : public SpellScriptLoader
{
public:
    the_everbloom_venom_spray() : SpellScriptLoader("the_everbloom_venom_spray") { }

    class the_the_everbloom_spells : public SpellScript
    {
        PrepareSpellScript(the_the_everbloom_spells);

        void VenomSpray()
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (Unit* l_Target = GetExplTargetUnit())
                {
                    for (int i = 0; i <= 12; i++)
                        l_Caster->CastSpell(l_Target, eEverbloomSpells::SpellVenomSprayDamage, true);
                }
            }
        }

        void Register()
        {
            AfterCast += SpellCastFn(the_the_everbloom_spells::VenomSpray);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new the_the_everbloom_spells();
    }
};

// Living Leaves
class the_everbloom_living_leaves : public AreaTriggerEntityScript
{
public:
    the_everbloom_living_leaves() : AreaTriggerEntityScript("the_everbloom_living_leaves")
    {
    }

    uint32 p_Diff = 1 * TimeConstants::IN_MILLISECONDS;
    std::set<uint64> m_Targets;

    void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
    {
        if (p_Diff <= p_Time)
        {
            std::list<Player*> l_PlayerList;

            JadeCore::AnyPlayerInObjectRangeCheck check(p_AreaTrigger, 1.2f);
            JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(p_AreaTrigger, l_PlayerList, check);
            p_AreaTrigger->VisitNearbyObject(1.2f, searcher);

            if (!l_PlayerList.empty())
            {
                for (Player* l_Target : l_PlayerList)
                {
                    if (!l_Target->HasAura(eEverbloomSpells::SpellLivingLeavesEffect))
                    {
                        l_Target->AddAura(eEverbloomSpells::SpellLivingLeavesEffect, l_Target);

                        m_Targets.insert(l_Target->GetGUID());
                    }
                }
            }

            p_Diff = 2 * TimeConstants::IN_MILLISECONDS;
        }
        else
        {
            p_Diff -= p_Time;
        }
    }

    void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time)
    {
        for (auto l_Guid : m_Targets)
        {
            Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
            if (l_Target)
                l_Target->RemoveAura(eEverbloomSpells::SpellLivingLeavesEffect);
        }
    }

    the_everbloom_living_leaves* GetAI() const
    {
        return new the_everbloom_living_leaves();
    }
};

// Living Leaves Dummy
class the_everbloom_living_leaves_dummy : public SpellScriptLoader
{
public:
    the_everbloom_living_leaves_dummy() : SpellScriptLoader("the_everbloom_living_leaves_dummy") { }

    class the_everbloom_spells : public SpellScript
    {
        PrepareSpellScript(the_everbloom_spells);

        void HandleDummy(SpellEffIndex effIndex)
        {
            if (Unit* l_Caster = GetCaster())
            {
                for (int i = 0; i < 5; i++)
                {
                    Position l_Pos;
                    l_Caster->GetRandomNearPosition(l_Pos, 15.0f);

                    l_Caster->SummonCreature(eEverbloomCreature::TriggerLivingLeaves, l_Pos, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 15 * TimeConstants::IN_MILLISECONDS);
                }
            }
        }

        void Register()
        {
            OnEffectLaunch += SpellEffectFn(the_everbloom_spells::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new the_everbloom_spells();
    }
};

// Living Leaves exectue
class the_everbloom_living_leaves_executre : public CreatureScript
{
public:
    the_everbloom_living_leaves_executre() : CreatureScript("the_everbloom_living_leaves_executre") { }

    struct the_everbloom_creaturesAI : public ScriptedAI
    {
        the_everbloom_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
        }

        uint32 m_Time;

        void Reset() override
        {
            events.Reset();
            m_Time = 1500;

            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS,  eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);

            me->CastSpell(me, eEverbloomSpells::SpellLivingLeavesAreaTrigger);
        }
        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (m_Time <= p_Diff)
            {
                std::list<Player*> l_PlayerList;
                l_PlayerList.clear();

                me->GetPlayerListInGrid(l_PlayerList, 2.0f);

                if (!l_PlayerList.empty())
                {
                    for (auto itr : l_PlayerList)
                    {
                        if (!itr->HasAura(eEverbloomSpells::SpellLivingLeavesEffect))
                        {
                            itr->CastSpell(itr, eEverbloomSpells::SpellLivingLeavesEffect, true);

                            AuraPtr aura = itr->GetAura(eEverbloomSpells::SpellLivingLeavesEffect);
                        }
                    }
                }

                m_Time = 500;
            }
            else
            {
                m_Time -= p_Diff;
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new the_everbloom_creaturesAI(pCreature);
    }
};

// Bounding Whirl - 172576 
class the_everbloom_bounding_whirl_dummy : public SpellScriptLoader
{
public:
    the_everbloom_bounding_whirl_dummy() : SpellScriptLoader("the_everbloom_bounding_whirl_dummy") { }

    class the_everbloom_spells : public SpellScript
    {
        PrepareSpellScript(the_everbloom_spells);

        void HandleDummy(SpellEffIndex effIndex)
        {
            if (!GetCaster())
                return;

            if (Creature* l_Caster = GetCaster()->ToCreature())
            {
                if (Unit* l_Target = GetHitUnit())
                {
                    l_Caster->CastSpell(l_Target, eEverbloomSpells::SpellBoundingWhirlJump);

                    if (l_Caster->IsAIEnabled)
                        l_Caster->AI()->DoAction(eEverbloomActions::ActionBoundingWhirlAura);
                }
            }
        }

        void Register()
        {
            OnEffectHit += SpellEffectFn(the_everbloom_spells::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new the_everbloom_spells();
    }
};

// Poisonous Claws - 169657 
class the_everbloom_poisonous_claws : public SpellScriptLoader
{
public:
    the_everbloom_poisonous_claws() : SpellScriptLoader("the_everbloom_poisonous_claws") { }

    class the_everbloom_spells : public SpellScript
    {
        PrepareSpellScript(the_everbloom_spells);

        void OnHitApply()
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (Unit* l_Target = GetHitUnit())
                    l_Caster->AddAura(eEverbloomSpells::SpellInfectedWounds, l_Target);
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(the_everbloom_spells::OnHitApply);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new the_everbloom_spells();
    }
};

void AddSC_the_everbloom()
{
    /*Creatures*/
    // -- Archmage Sol: 
    new the_everbloom_infested_icecaller();
    new the_everbloom_addled_arcanomancer();
    new the_everbloom_putrid_pyromancer();

    // -- Everbloom General: 
    new the_everbloom_dreadpital_toxin();
    new the_everbloom_tender();
    new the_everbloom_melded_berserker();
    new the_everbloom_gnarlroot();
    new the_everbloom_verdant_mandragora();
    new the_everbloom_mender();
    new the_everbloom_twisted_abomination();

    /*Spells*/
    new the_everbloom_spore_breath_infect();
    new the_everbloom_venom_spray();
    new the_everbloom_living_leaves_dummy();
    new the_everbloom_living_leaves_executre();
    new the_everbloom_poisonous_claws();
    new the_everbloom_bounding_whirl_dummy();
}