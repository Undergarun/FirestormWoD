#include "instance_skyreach.h"



// Spinning Blade - 153544
class spell_SpinningBlade : public SpellScriptLoader
{
public:
    spell_SpinningBlade() 
        : SpellScriptLoader("spell_SpinningBlade")
    {
    }

    enum class SpinningBladeSpells : uint32
    {
        SPINNING_BLADE_2 = 153123,  // Launched after 1s of SPINNING_BLADE
        SPINNING_BLADE_3 = 153586,
        SPINNING_BLADE_4 = 153535,
        SPINNING_BLADE_5 = 153536,
        SPINNING_BLADE_6 = 153537,
        SPINNING_BLADE_7 = 153538,
        SPINNING_BLADE_8 = 153588,
        SPINNING_BLADE_9 = 153583,  // 2:41:02
        SPINNING_BLADE_10 = 153585,
    };

    class spell_SpinningBlade_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_SpinningBlade_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (GetCaster() && GetHitUnit())
            {
                // Spinning Blade AreaTrigger
                GetCaster()->CastSpell(GetHitUnit(), uint32(SpinningBladeSpells::SPINNING_BLADE_9), true);
            }
        }

        void Register()
        {
            OnEffectLaunchTarget += SpellEffectFn(spell_SpinningBlade_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_SpinningBlade_SpellScript();
    }
};

class mob_SolarFamiliar : public CreatureScript
{
public:
    // Entry: 76097
    mob_SolarFamiliar()
        : CreatureScript("mob_SolarFamiliar")
    {
    }

    enum class SolarFamiliarSpells
    {
        EMPOWER = 159290,   // 2:40:45 - 2:40:52
        FLASH_BANG = 152953, // Something like every 10 seconds.
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_SolarFamiliarAI(creature);
    }

    struct mob_SolarFamiliarAI : public ScriptedAI
    {
        mob_SolarFamiliarAI(Creature* creature) : ScriptedAI(creature),
        m_instance(creature->GetInstanceScript()),
        m_events()
        {
        }

        InstanceScript* m_instance;
        EventMap m_events;
    };
};

class mob_SoaringChrakramMaster : public CreatureScript
{
public:
    // Entry: 76132
    mob_SoaringChrakramMaster()
        : CreatureScript("mob_SoaringChrakramMaster")
    {
    }

    enum class SoaringChrakramMasterSpells
    {
        THROW_CHAKRAM_YELLOW = 169689,  // 2:40:57 - 2:40:58 2:41:00 - 2:41:06
        THROW_CHAKRAM_WHITE = 169690,   // 2:40:48 - 2:40:52 - 2:40:56 - 2:40:57
        THROW_CHAKRAM_2 = 169687,       // Launched with the two previous ones.
        THROW_CHAKRAM_DNT = 178612,
        SPINNING_BLADE = 153544,    // 2:40:16 - 2:40:35 - FIXME: NOT WORKING
        SPINNING_BLADE_2 = 153123,  // Launched after 1s of SPINNING_BLADE
        SPINNING_BLADE_3 = 153586,
        SPINNING_BLADE_4 = 153535,
        SPINNING_BLADE_5 = 153536,
        SPINNING_BLADE_6 = 153537,
        SPINNING_BLADE_7 = 153538,
        SPINNING_BLADE_8 = 153588,
        SPINNING_BLADE_9 = 153583,
        SPINNING_BLADE_10 = 153585,
        BLADE_DANCE = 153581,       // 2:40:56 - 2:41:17 - FIXME: NOT WORKING
        WIELD_CHAKRAMS = 170378,
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_SoaringChrakramMasterAI(creature);
    }

    struct mob_SoaringChrakramMasterAI : public ScriptedAI
    {
        mob_SoaringChrakramMasterAI(Creature* creature) : ScriptedAI(creature),
        m_instance(creature->GetInstanceScript()),
        m_events()
        {
        }

        InstanceScript* m_instance;
        EventMap m_events;
    };
};

class mob_HeraldOfSunrise : public CreatureScript
{
public:
    // Entry: 78933
    mob_HeraldOfSunrise()
        : CreatureScript("mob_HeraldOfSunrise")
    {
    }

    enum class HeraldOfSunriseSpells
    {
        SOLAR_ZONE = 163330,    // 2:40:57 - 2:41:11 - FIXME: NOT WORKING
        SOLAR_ZONE_1 = 160935,
        SOLAR_ZONE_2 = 160281,  // 2:40:58 - 2:40:59 - 2:41:02
        MAGMA_EXPLOSION_SUPREME = 152864, // -FIXME: NOT WORKING
        SOLAR_HEAL = 152893,    // 2:41:08
        FLASH_HEAL = 152894,
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_HeraldOfSunriseAI(creature);
    }

    struct mob_HeraldOfSunriseAI : public ScriptedAI
    {
        mob_HeraldOfSunriseAI(Creature* creature) : ScriptedAI(creature),
        m_instance(creature->GetInstanceScript()),
        m_events()
        {
        }

        InstanceScript* m_instance;
        EventMap m_events;
    };
};

class mob_WhirlingDervish : public CreatureScript
{
public:
    // Entry: 77605
    mob_WhirlingDervish()
        : CreatureScript("mob_WhirlingDervish")
    {
    }

    enum class WhirlingDervishSpells
    {
        TWISTER_DNT = 178617,
        STORM = 156515,             // FIXME NOT WORKING
        STORM_VISUAL = 156840,
        STORM_DMG = 156841,
        DERVISH = 153905,           // FIXME NOT WORKING
        DERVISH_DMG = 153907,
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_WhirlingDervishAI(creature);
    }

    struct mob_WhirlingDervishAI : public ScriptedAI
    {
        mob_WhirlingDervishAI(Creature* creature) : ScriptedAI(creature),
        m_instance(creature->GetInstanceScript()),
        m_events()
        {
        }

        InstanceScript* m_instance;
        EventMap m_events;
    };
};

class mob_BloodedBladeFeather : public CreatureScript
{
public:
    // Entry: 76205
    mob_BloodedBladeFeather()
        : CreatureScript("mob_BloodedBladeFeather")
    {
    }

    enum class BloodedBladefeatherSpells
    {
        PIERCING_RUSH_1 = 165731,   // 2:40:28 - 2:40:55 - FIXME: NOT WORKING
        PIERCING_RUSH_2 = 165733,   // 2:40:28 - 2:40:55
        PIERCING_RUSH_3 = 165732,   // 
        DESPAWN_AREA_TRIGGERS = 138175,
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_BloodedBladeFeatherAI(creature);
    }

    struct mob_BloodedBladeFeatherAI : public ScriptedAI
    {
        mob_BloodedBladeFeatherAI(Creature* creature) : ScriptedAI(creature),
        m_instance(creature->GetInstanceScript()),
        m_events()
        {
        }

        InstanceScript* m_instance;
        EventMap m_events;
    };
};

class mob_InitiateOfTheRisingSun : public CreatureScript
{
public:
    // Entry: 79466
    mob_InitiateOfTheRisingSun()
        : CreatureScript("mob_InitiateOfTheRisingSun")
    {
    }

    enum class InitiateOfTheRisingSunSpells
    {
        MAGMA_EXPLOSION_SUPREME = 152864, // FIXME: NOT WORKING
        SOLAR_SHOWER = 160274,
        SOLAR_SHOWER_2 = 160275,
        FLASH_HEAL = 152894,
        CONJURE_SUN_ORB_DNT = 178618, // Visual to do when closed to sun orbs.
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_InitiateOfTheRisingSunAI(creature);
    }

    struct mob_InitiateOfTheRisingSunAI : public ScriptedAI
    {
        mob_InitiateOfTheRisingSunAI(Creature* creature) : ScriptedAI(creature),
        m_instance(creature->GetInstanceScript()),
        m_events()
        {
        }

        InstanceScript* m_instance;
        EventMap m_events;
    };
};

class mob_DivingChakramSpinner : public CreatureScript
{
public:
    // Entry: 76116
    mob_DivingChakramSpinner()
        : CreatureScript("mob_DivingChakramSpinner")
    {
    }

    enum class DivingChakramSpinnerSpells
    {
        THROW_CHAKRAM_YELLOW = 169689,  // 2:40:57 - 2:40:58 2:41:00 - 2:41:06
        THROW_CHAKRAM_WHITE = 169690,   // 2:40:48 - 2:40:52 - 2:40:56 - 2:40:57
        THROW_CHAKRAM_2 = 169687,       // Launched with the two previous ones.
        THROW_CHAKRAM_DNT = 178612,
        SPINNING_BLADE = 153544,    // 2:40:16 - 2:40:35 FIXME: NOT WORKING
    };

    enum class DivingChakramSpinnerEvents : uint32
    {
        THROW_CHAKRAM = 1,
        SPINNING_BLADE = 2
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_DivingChakramSpinnerAI(creature);
    }

    struct mob_DivingChakramSpinnerAI : public ScriptedAI
    {
        mob_DivingChakramSpinnerAI(Creature* creature) : ScriptedAI(creature),
        m_instance(creature->GetInstanceScript()),
        m_events()
        {
        }

        void Reset()
        {
            m_events.Reset();
        }

        void EnterCombat(Unit* who)
        {
            m_events.ScheduleEvent(uint32(DivingChakramSpinnerEvents::THROW_CHAKRAM), urand(2000, 3000));
            m_events.ScheduleEvent(uint32(DivingChakramSpinnerEvents::SPINNING_BLADE), urand(8000, 10000));
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;

            m_events.Update(diff);

            while (uint32 eventId = m_events.ExecuteEvent())
            {
                switch (eventId)
                {
                case uint32(DivingChakramSpinnerEvents::THROW_CHAKRAM):
                    m_events.ScheduleEvent(uint32(DivingChakramSpinnerEvents::THROW_CHAKRAM), urand(2000, 3000));

                    if (urand(0, 1) == 0)
                        me->CastSpell(me->getVictim(), uint32(DivingChakramSpinnerSpells::THROW_CHAKRAM_YELLOW));
                    else
                        me->CastSpell(me->getVictim(), uint32(DivingChakramSpinnerSpells::THROW_CHAKRAM_WHITE));
                    me->CastSpell(me->getVictim(), uint32(DivingChakramSpinnerSpells::THROW_CHAKRAM_2)); // Always launched with the two others.
                    break;
                case uint32(DivingChakramSpinnerEvents::SPINNING_BLADE):
                    m_events.ScheduleEvent(uint32(DivingChakramSpinnerEvents::SPINNING_BLADE), urand(8000, 10000));
                    if (Player* l_plr = InstanceSkyreach::SelectRandomPlayerIncludedTank(me, 45.0f))
                        me->CastSpell(l_plr, uint32(DivingChakramSpinnerSpells::SPINNING_BLADE));
                    break;
                default:
                    break;
                }

                // If we cast something, we don't want to execute the other events.
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
            }

            DoMeleeAttackIfReady();
        }

        InstanceScript* m_instance;
        EventMap m_events;
    };
};

class mob_BlindingSolarFlare : public CreatureScript
{
public:
    // Entry: 79462
    mob_BlindingSolarFlare()
        : CreatureScript ("mob_BlindingSolarFlare")
    {
    }

    enum class BlindingSolarFlareSpells : uint32
    {
        SOLAR_WRATH = 157020,           // 2:40:56 - 2:41:00 - 2:41:05 - 2:41:08
        SOLAR_DETONATION_2 = 160282,      // 2:41:05
        SOLAR_DETONATION = 160288,    // 2:40:59 - 2:41:12
    };

    enum class BlindingSolarFlareEvents : uint32
    {
        SOLAR_WRATH = 1,
        SOLAR_DETONATION = 2
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_BlindingSolarFlareAI(creature);
    }

    struct mob_BlindingSolarFlareAI : public ScriptedAI
    {
        mob_BlindingSolarFlareAI(Creature* creature) : ScriptedAI(creature),
        m_instance (creature->GetInstanceScript()),
        m_events ()
        {
        }

        void Reset()
        {
            m_events.Reset();
        }

        void EnterCombat(Unit* who)
        {
            m_events.ScheduleEvent(uint32(BlindingSolarFlareEvents::SOLAR_WRATH), urand(3000, 5000));
            m_events.ScheduleEvent(uint32(BlindingSolarFlareEvents::SOLAR_DETONATION), urand(10000, 14000));
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;

            m_events.Update(diff);

            while (uint32 eventId = m_events.ExecuteEvent())
            {
                switch (eventId)
                {
                case uint32(BlindingSolarFlareEvents::SOLAR_WRATH):
                    m_events.ScheduleEvent(uint32(BlindingSolarFlareEvents::SOLAR_WRATH), urand(3000, 5000));
                    me->CastSpell(me->getVictim(), uint32(BlindingSolarFlareSpells::SOLAR_WRATH));
                    break;
                case uint32(BlindingSolarFlareEvents::SOLAR_DETONATION):
                    m_events.ScheduleEvent(uint32(BlindingSolarFlareEvents::SOLAR_DETONATION), urand(10000, 14000));
                    if (Player* l_plr = InstanceSkyreach::SelectRandomPlayerIncludedTank(me, 45.0f))
                        me->CastSpell(l_plr, uint32(BlindingSolarFlareSpells::SOLAR_DETONATION));
                    break;
                default:
                    break;
                }

                // If we cast something, we don't want to execute the other events.
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
            }

            DoMeleeAttackIfReady();
        }

        InstanceScript* m_instance;
        EventMap m_events;
    };
};

void AddSC_mob_instance_skyreach()
{
    // Spells.
    new spell_SpinningBlade();

    // Mobs.
    new mob_BlindingSolarFlare();
    new mob_DivingChakramSpinner();
}