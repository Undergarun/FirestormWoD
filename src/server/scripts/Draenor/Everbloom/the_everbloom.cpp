////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2016 Millenium-studio SARL
///  All Rights Reserved.
///  Coded by Davethebrave
////////////////////////////////////////////////////////////////////////////////

#include "the_everbloom.hpp"

/// Dreadpital toxin - 81864
class the_everbloom_mob_dreadpital_toxin : public CreatureScript
{
public:

    the_everbloom_mob_dreadpital_toxin() : CreatureScript("the_everbloom_mob_Dreadpital_Toxin") { }

    struct the_everbloom_mob_dreadpital_toxinAI : public ScriptedAI
    {
        the_everbloom_mob_dreadpital_toxinAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        enum eDreadpetalEvents
        {
            EventDreadpetalToxin = 1
        };

        enum eDreadpetalSpells
        {
            SpellDreadpetalToxin = 164886
        };

        void Reset()
        {
            events.Reset();
            me->setFaction(HostileFaction);
        }

        void EnterCombat(Unit* /*p_Attacker*/)
        {
            events.ScheduleEvent(eDreadpetalEvents::EventDreadpetalToxin, urand(8 * TimeConstants::IN_MILLISECONDS, 14 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case eDreadpetalEvents::EventDreadpetalToxin:
                if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 20.0f, true))
                    me->CastSpell(l_Target, eDreadpetalSpells::SpellDreadpetalToxin);
                events.ScheduleEvent(eDreadpetalEvents::EventDreadpetalToxin, urand(8 * TimeConstants::IN_MILLISECONDS, 14 * TimeConstants::IN_MILLISECONDS));
                break;
            default:
                break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_mob_dreadpital_toxinAI(p_Creature);
    }
};

/// Everbloom Tender - 81985
class the_everbloom_mob_everbloom_tender : public CreatureScript
{
public:

    the_everbloom_mob_everbloom_tender() : CreatureScript("the_everbloom_mob_everbloom_tender") { }

    struct the_everbloom_mob_everbloom_tenderAI : public ScriptedAI
    {
        the_everbloom_mob_everbloom_tenderAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eTenderEvents
        {
            EventDancingThorns = 1,
            EventEnragedGrowth
        };

        enum eTenderSpells
        {
            SpellDancingThorns = 164973,
            SpellEnragedGrowth = 165213,
            SpellSolarChannel = 170594
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            me->CastSpell(me, eTenderSpells::SpellSolarChannel);
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            events.ScheduleEvent(eTenderEvents::EventDancingThorns, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eTenderEvents::EventEnragedGrowth, urand(7 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case eTenderEvents::EventDancingThorns:
                if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                    me->CastSpell(l_Target, eTenderSpells::SpellDancingThorns);
                events.ScheduleEvent(eTenderEvents::EventDancingThorns, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
                break;
            case eTenderEvents::EventEnragedGrowth:
                if (Creature* l_Petal = me->FindNearestCreature(eEverbloomCreature::CreatureDreadpetalToxin, 20.0f, true))
                    me->CastSpell(l_Petal, eTenderSpells::SpellEnragedGrowth);
                events.ScheduleEvent(eTenderEvents::EventEnragedGrowth, urand(12 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));
                break;
            default:
                break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_mob_everbloom_tenderAI(p_Creature);
    }
};

/// Melder Berserker - 86372
class the_everbloom_mob_melded_berserker : public CreatureScript
{
public:

    the_everbloom_mob_melded_berserker() : CreatureScript("the_everbloom_mob_melded_berserker") { }

    struct the_everbloom_mob_melded_berserkerAI : public ScriptedAI
    {
        the_everbloom_mob_melded_berserkerAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eMeldedBerserkerEvents
        {
            EventVileBreath = 1,
            EventBoundingWhirl,
            EventBoundingWhirl2
        };

        enum eMeldedBerserkerSpells
        {
            SpellEnrage = 38166,
            SpellVileBreath = 172588,
            SpellBoundingWhirlAura = 172578,
            SpellBoundingWhirlJump = 172577
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            me->CastSpell(me, eMeldedBerserkerSpells::SpellEnrage);
            events.ScheduleEvent(eMeldedBerserkerEvents::EventVileBreath, urand(10 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eMeldedBerserkerEvents::EventBoundingWhirl, 20 * TimeConstants::IN_MILLISECONDS);
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
            case eEverbloomActions::ActionBoundingWhirlAura:
                me->CastSpell(me, eMeldedBerserkerSpells::SpellBoundingWhirlAura);
                break;
            default:
                break;
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case eMeldedBerserkerEvents::EventVileBreath:
                if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                    me->CastSpell(l_Target, eMeldedBerserkerSpells::SpellVileBreath);
                events.ScheduleEvent(eMeldedBerserkerEvents::EventVileBreath, urand(12 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
                break;
            case eMeldedBerserkerEvents::EventBoundingWhirl:
                if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                    me->CastSpell(l_Target, eMeldedBerserkerSpells::SpellBoundingWhirlJump);
                events.ScheduleEvent(eMeldedBerserkerEvents::EventBoundingWhirl, 20 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eMeldedBerserkerEvents::EventBoundingWhirl2, 2 * TimeConstants::IN_MILLISECONDS);
                break;
            case eMeldedBerserkerEvents::EventBoundingWhirl2:
                me->CastSpell(me, eMeldedBerserkerSpells::SpellBoundingWhirlAura);
                break;
            default:
                break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_mob_melded_berserkerAI(p_Creature);
    }
};

/// Everbloom Gnarloot - 81984
class the_everbloom_mob_gnarlroot : public CreatureScript
{
public:

    the_everbloom_mob_gnarlroot() : CreatureScript("the_everbloom_mob_gnarlroot") { }

    struct the_everbloom_mob_gnarlrootAI : public ScriptedAI
    {
        the_everbloom_mob_gnarlrootAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eGnarlrootEvents
        {
            EventGasp = 1,
            EventBarrageOfLeaves
        };

        enum eGnarlrootSpells
        {
            SpellGasp = 169714,
            SpellLivingLeavesDummy = 169494
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            events.ScheduleEvent(eGnarlrootEvents::EventGasp, urand(10 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eGnarlrootEvents::EventBarrageOfLeaves, 15 * TimeConstants::IN_MILLISECONDS);
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case eGnarlrootEvents::EventGasp:
                if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                    me->CastSpell(l_Target, eGnarlrootSpells::SpellGasp);
                events.ScheduleEvent(eGnarlrootEvents::EventGasp, urand(10 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
                break;
            case eGnarlrootEvents::EventBarrageOfLeaves:
                me->CastSpell(me, eGnarlrootSpells::SpellLivingLeavesDummy);
                events.ScheduleEvent(eGnarlrootEvents::EventBarrageOfLeaves, 30 * TimeConstants::IN_MILLISECONDS);
                break;
            default:
                break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_mob_gnarlrootAI(p_Creature);
    }
};

/// Verdant Mandragora - 81983
class the_everbloom_mob_verdant_mandragora : public CreatureScript
{
public:

    the_everbloom_mob_verdant_mandragora() : CreatureScript("the_everbloom_mob_verdant_mandragora") { }

    struct the_everbloom_mob_verdant_mandragoraAI : public ScriptedAI
    {
        the_everbloom_mob_verdant_mandragoraAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eMandragoraEvents
        {
            EventVirulendGasp = 1
        };

        enum eMandragoraSpells
        {
            SpellVirulendGasp = 165093
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            events.ScheduleEvent(eMandragoraEvents::EventVirulendGasp, urand(8 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case eMandragoraEvents::EventVirulendGasp:
                if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                    me->CastSpell(l_Target, eMandragoraSpells::SpellVirulendGasp);
                events.ScheduleEvent(eMandragoraEvents::EventVirulendGasp, urand(7 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
                break;
            default:
                break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_mob_verdant_mandragoraAI(p_Creature);
    }
};

/// Everbloom Mender - 81820
class the_everbloom_mob_everbloom_mender : public CreatureScript
{
public:

    the_everbloom_mob_everbloom_mender() : CreatureScript("the_everbloom_mob_everbloom_mender") { }

    struct the_everbloom_mob_everbloom_menderAI : public ScriptedAI
    {
        the_everbloom_mob_everbloom_menderAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eEverbloomMenderEvents
        {
            EventChokingVines = 1,
            EventHealingWaters
        };

        enum eEverbloomMenderSpells
        {
            SpellChokingVines = 164965,
            SpellHealingWaters = 164887
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            events.ScheduleEvent(eEverbloomMenderEvents::EventChokingVines, 18 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eEverbloomMenderEvents::EventHealingWaters, 10 * TimeConstants::IN_MILLISECONDS);
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case eEverbloomMenderEvents::EventChokingVines:
            {
                if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                    me->CastSpell(l_Target, eEverbloomMenderSpells::SpellChokingVines);
                events.ScheduleEvent(eEverbloomMenderEvents::EventChokingVines, 18 * TimeConstants::IN_MILLISECONDS);
                break;
            }
            case eEverbloomMenderEvents::EventHealingWaters:
            {
                if (Unit* l_FriendlyUnit = DoSelectLowestHpFriendly(85)) // heal
                    me->CastSpell(l_FriendlyUnit, eEverbloomMenderSpells::SpellHealingWaters);
                events.ScheduleEvent(eEverbloomMenderEvents::EventHealingWaters, 10 * TimeConstants::IN_MILLISECONDS);
                break;
            }
            default:
                break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_mob_everbloom_menderAI(p_Creature);
    }
};

/// Twisted Abomination - 84767
class the_everbloom_mob_twisted_abomination : public CreatureScript
{
public:

    the_everbloom_mob_twisted_abomination() : CreatureScript("the_everbloom_mob_twisted_abomination") { }

    struct the_everbloom_mob_twisted_abominationAI : public ScriptedAI
    {
        the_everbloom_mob_twisted_abominationAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eTwistedAbominationEvents
        {
            EventNoxiousEruption = 1,
            EventPoisonousClaws
        };

        enum eTwistedAbominationSpells
        {
            SpellAbominationNoxiousErupt = 169445,
            SpellPoisonousClaws = 169657
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            events.ScheduleEvent(eTwistedAbominationEvents::EventNoxiousEruption, 20 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eTwistedAbominationEvents::EventPoisonousClaws, urand(8 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case eTwistedAbominationEvents::EventNoxiousEruption:
            {
                me->CastSpell(me, eTwistedAbominationSpells::SpellAbominationNoxiousErupt);
                events.ScheduleEvent(eTwistedAbominationEvents::EventNoxiousEruption, 20 * TimeConstants::IN_MILLISECONDS);
                break;
            }
            case eTwistedAbominationEvents::EventPoisonousClaws:
            {
                if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                    me->CastSpell(l_Target, eTwistedAbominationSpells::SpellPoisonousClaws);
                events.ScheduleEvent(eTwistedAbominationEvents::EventPoisonousClaws, urand(8 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
                break;
            }
            default:
                break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_mob_twisted_abominationAI(p_Creature);
    }
};

/// Infested Icecaller - 84989
class the_everbloom_mob_infested_icecaller : public CreatureScript
{
public:

    the_everbloom_mob_infested_icecaller() : CreatureScript("the_everbloom_mob_infested_icecaller") { }

    struct the_everbloom_mob_infested_icecallerAI : public ScriptedAI
    {
        the_everbloom_mob_infested_icecallerAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eInfestedIcecallerEvents
        {
            EventFrozenSnap = 1,
            EventFrostbolt
        };

        enum eInfestedIcecallerSpells
        {
            SpellFrostbolt = 169840,
            SpellIceAura = 169831,
            SpellMindRoot = 169828
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            me->AddAura(eInfestedIcecallerSpells::SpellIceAura, me);
            me->AddAura(eInfestedIcecallerSpells::SpellMindRoot, me);
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            events.ScheduleEvent(eInfestedIcecallerEvents::EventFrozenSnap, urand(10 * TimeConstants::IN_MILLISECONDS, 17 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eInfestedIcecallerEvents::EventFrostbolt, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case eInfestedIcecallerEvents::EventFrostbolt:
                if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                    me->CastSpell(l_Target, eInfestedIcecallerSpells::SpellFrostbolt);
                events.ScheduleEvent(eInfestedIcecallerEvents::EventFrostbolt, 6 * TimeConstants::IN_MILLISECONDS);
                break;
            case eInfestedIcecallerEvents::EventFrozenSnap:
                for (uint8 i = 0; i < 3; i++)
                {
                    Position l_Pos;
                    me->GetRandomNearPosition(l_Pos, 20.0f);
                    me->SummonCreature(eEverbloomCreature::CreatureFrozenSnap, l_Pos, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 4 * TimeConstants::IN_MILLISECONDS);
                }
                events.ScheduleEvent(eInfestedIcecallerEvents::EventFrozenSnap, urand(30 * TimeConstants::IN_MILLISECONDS, 50 * TimeConstants::IN_MILLISECONDS));
                break;
            default:
                break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_mob_infested_icecallerAI(p_Creature);
    }
};

///  Addled Arcanomancer - 84990
class the_everbloom_mob_addled_arcanomancer : public CreatureScript
{
public:

    the_everbloom_mob_addled_arcanomancer() : CreatureScript("the_everbloom_mob_addled_arcanomancer") { }

    struct the_everbloom_mob_addled_arcanomancerAI : public ScriptedAI
    {
        the_everbloom_mob_addled_arcanomancerAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eAddledArcanonmacerEvents
        {
            EventArcaneBlast = 1
        };

        enum eAddledArcanmoacerSpells
        {
            SpellArcaneBlast = 169825,
            SpellMindRoot = 169828
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            me->AddAura(eAddledArcanmoacerSpells::SpellMindRoot, me);
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            events.ScheduleEvent(eAddledArcanonmacerEvents::EventArcaneBlast, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case eAddledArcanonmacerEvents::EventArcaneBlast:
                if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0, true))
                    me->CastSpell(l_Random, eAddledArcanmoacerSpells::SpellArcaneBlast);
                events.ScheduleEvent(eAddledArcanonmacerEvents::EventArcaneBlast, 6 * TimeConstants::IN_MILLISECONDS);
            default:
                break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_mob_addled_arcanomancerAI(p_Creature);
    }
};

/// Putrid Pyromancer - 84957
class the_everbloom_mob_putrid_pyromancer : public CreatureScript
{
public:

    the_everbloom_mob_putrid_pyromancer() : CreatureScript("the_everbloom_mob_putrid_pyromancer") { }

    struct the_everbloom_mob_putrid_pyromancerAI : public ScriptedAI
    {
        the_everbloom_mob_putrid_pyromancerAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum ePutridPyromancerEvents
        {
            EventDragonsBreath = 1,
            EventFireball
        };

        enum ePutridPyromancerSpells
        {
            SpellDragonsBreath = 169843,
            SpellFireBall = 169839,
            SpellMindRoot = 169828
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            me->AddAura(ePutridPyromancerSpells::SpellMindRoot, me);
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            events.ScheduleEvent(ePutridPyromancerEvents::EventDragonsBreath, urand(14 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(ePutridPyromancerEvents::EventFireball, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case ePutridPyromancerEvents::EventDragonsBreath:
                if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                    me->CastSpell(l_Random, ePutridPyromancerSpells::SpellDragonsBreath);
                events.ScheduleEvent(ePutridPyromancerEvents::EventDragonsBreath, urand(40 * TimeConstants::IN_MILLISECONDS, 50 * TimeConstants::IN_MILLISECONDS));
                break;
            case ePutridPyromancerEvents::EventFireball:
                if (Unit* l_Victim = me->getVictim())
                    me->CastSpell(l_Victim, ePutridPyromancerSpells::SpellFireBall);
                events.ScheduleEvent(ePutridPyromancerEvents::EventFireball, 6 * TimeConstants::IN_MILLISECONDS);
                break;
            default:
                break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_mob_putrid_pyromancerAI(p_Creature);
    }
};

/// Bounding Whirl - 172576 
class the_everbloom_spell_bounding_whirl_dummy : public SpellScriptLoader
{
public:

    the_everbloom_spell_bounding_whirl_dummy() : SpellScriptLoader("the_everbloom_spell_bounding_whirl_dummy") { }

    class the_everbloom_spell_bounding_whirl_dummy_SpellScript : public SpellScript
    {
        PrepareSpellScript(the_everbloom_spell_bounding_whirl_dummy_SpellScript)

        enum eBoundingWhirlSpells
        {
            SpellBoundingWhirlAura = 172578,
            SpellBoundingWhirlJump = 172577
        };

        void HandleDummy(SpellEffIndex /*p_EffIndex*/)
        {
            if (!GetCaster() && !GetHitUnit())
                return;

            if (Creature* l_Caster = GetCaster()->ToCreature())
            {
                if (Unit* l_Target = GetHitUnit())
                {
                    l_Caster->CastSpell(l_Target, eBoundingWhirlSpells::SpellBoundingWhirlJump);

                    if (l_Caster->IsAIEnabled)
                        l_Caster->AI()->DoAction(eEverbloomActions::ActionBoundingWhirlAura);
                }
            }
        }

        void Register()
        {
            OnEffectHit += SpellEffectFn(the_everbloom_spell_bounding_whirl_dummy_SpellScript::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new the_everbloom_spell_bounding_whirl_dummy_SpellScript();
    }
};

/// Poisonous Claws - 169657 
class the_everbloom_spell_poisonous_claws : public SpellScriptLoader
{
public:

    the_everbloom_spell_poisonous_claws() : SpellScriptLoader("the_everbloom_spell_poisonous_claws") { }

    class the_everbloom_spell_poisonous_claws_SpellScript : public SpellScript
    {
        PrepareSpellScript(the_everbloom_spell_poisonous_claws_SpellScript)

        enum eInfectedWoundsSpells
        {
            SpellInfectedWounds = 169658
        };

        void OnHitApply()
        {
            if (!GetCaster() && !GetHitUnit())
                return;

            GetCaster()->AddAura(eInfectedWoundsSpells::SpellInfectedWounds, GetHitUnit());
        }

        void Register()
        {
            OnHit += SpellHitFn(the_everbloom_spell_poisonous_claws_SpellScript::OnHitApply);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new the_everbloom_spell_poisonous_claws_SpellScript();
    }
};

#ifndef __clang_analyzer__
void AddSC_the_everbloom()
{
    new the_everbloom_mob_infested_icecaller();     ///< 84989
    new the_everbloom_mob_addled_arcanomancer();    ///< 84990
    new the_everbloom_mob_putrid_pyromancer();      ///< 84957
    new the_everbloom_mob_dreadpital_toxin();       ///< 81864
    new the_everbloom_mob_melded_berserker();       ///< 86372
    new the_everbloom_mob_gnarlroot();              ///< 81984
    new the_everbloom_mob_verdant_mandragora();     ///< 81983
    new the_everbloom_mob_everbloom_mender();       ///< 81820
    new the_everbloom_mob_everbloom_tender();       ///< 81985
    new the_everbloom_mob_twisted_abomination();    ///< 84767
    new the_everbloom_spell_poisonous_claws();      ///< 169657    
}
#endif
