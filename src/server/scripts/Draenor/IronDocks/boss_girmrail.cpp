////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "iron_docks.hpp"

enum eYells
{
    /// Noxx
    SayIntro            = 50,   ///< Is there killing to be done? Let me have 'em! (46128)
    SaySlay             = 3,    ///< Tick.. tick.. tick.. BOOM! (46129)
    SayDeath            = 4,    ///< I'm hit... (46127)
    SaySpell01          = 1,    ///< Now you ain't so tall no more! (46130)
    SaySpell02          = 2,    ///< You better step back. I'm calling in the bomb squad! (46131)
    SaySpell03          = 5,    ///< Time is money friend, and it looks like you out of both! (46132)
    ///Makogg
    SayIntroMakogg      = 60,   ///<  Hah! (45800)
    SayDeathMakogg      = 61,   ///<  This.. is.. far, from over.. (45799)
    SaySpell01Makogg    = 62    ///<  You will burn! (45801)
};

enum eSpells
{
    /// Makogg
    SpellFlamingSlashDummy      = 163665,
    SpellFlamingSlashUnkAura    = 163669,
    SpellFlamingSlashDamage     = 163668,
    SpellLavaSweepVisual        = 98873,    ///< HACK
    SpellLavaSweepDamage        = 165152,
    /// Ahri'ok dugru
    SpellBloodBolt              = 165122,
    SpellSanguineSphere         = 163689,
    SpellAbruptRestoration      = 163705,
    SpellTaintedBlood           = 163740,
    /// Noxx
    SpellGutShot                = 163334,
    SpellOgreTrapOpenTeeth      = 177391,
    SpellOgreTrapClosedTeeth    = 177396,
    SpellShreddingTendons       = 163276,
    SpellJumperCables           = 163376,
    /// Bombsquad
    SpellBigBoom                = 163379,
};

enum eEvents
{
    EventFlamingSlash   = 500,
    EventLavaSweep      = 501,
    /// DURGRU
    EventBloodBolt      = 502,
    EventSanguineSphere = 5654,
    EventTaintedBlood   = 504,
    /// Noxx
    EventGutShot        = 505,
    EventOgreTrap       = 506,
    EventJumperCables   = 507,
    EventBombSquad      = 509,
    /// Train Event
    EventBegin          = 942,
    EventProceed1       = 943,
    EventProceed2       = 945
};

enum eCreatures
{
    TriggerLavaSweep    = 95353,
    OgreTrap            = 88758,
    BombsquadTrigger    = 80875,
    Train               = 83673
};

enum eTalks
{
    TalkZoggosh05 = 14, ///< Sir.. he's out of ammunition. I'm reloading.. I'm reloading!! (44052)
    TalkKoramar05 = 27, ///< What is wrong with you?! keep firing you all! (46904)
    TalkKoramar06 = 28, ///< Arghh.. forget it, they'll never make it pass the tracks.. (unknown message) (46905)

    /// Event 3
    TalkZoggosh06 = 15, ///< Sir.. half of our army has been taken out. Don't you think we should.. (44053)
    TalkKoramar07 = 29  ///< That was an insignificant loss... let them exhaust themselves playing against our weak.. against those who're unworthy of serving in Black Hands army. That's all what Orc understand(46906)
};

class beforegrimrail_event : public BasicEvent
{
    public:
        explicit beforegrimrail_event(Unit* p_Unit, int p_Value) : m_Obj(p_Unit), m_Modifier(p_Value) { }

        bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/)
        {
            if (InstanceScript* l_Instance = m_Obj->GetInstanceScript())
            {
                if (Creature* l_Zoggosh = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataZuggosh)))
                {
                    if (Creature* l_Koramar = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataKoramar)))
                    {
                        if (Creature* l_Skulloc = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataSkulloc)))
                        {
                            if (l_Zoggosh->IsAIEnabled)
                            {
                                switch (m_Modifier)
                                {
                                    case 0:
                                        l_Koramar->AI()->Talk(eTalks::TalkKoramar05);
                                        l_Koramar->m_Events.AddEvent(new beforegrimrail_event(l_Koramar, 1), l_Koramar->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    case 1:
                                        l_Zoggosh->AI()->Talk(eTalks::TalkZoggosh05);
                                        l_Koramar->m_Events.AddEvent(new beforegrimrail_event(l_Koramar, 2), l_Koramar->m_Events.CalculateTime(10 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    case 2:
                                        l_Koramar->AI()->Talk(eTalks::TalkKoramar06);
                                        break;
                                    default:
                                        break;
                                }
                            }
                        }
                    }
                }
            }

            return true;
        }
    private:
        Unit* m_Obj;
        int m_Modifier;
};

class aftergrimrail_event : public BasicEvent
{
    public:
        explicit aftergrimrail_event(Unit* p_Unit, int p_Value) : m_Obj(p_Unit), m_Modifier(p_Value) { }

        bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/)
        {
            if (InstanceScript* l_Instance = m_Obj->GetInstanceScript())
            {
                if (Creature* l_Zoggosh = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataZuggosh)))
                {
                    if (Creature* l_Koramar = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataKoramar)))
                    {
                        if (Creature* l_Skulloc = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataSkulloc)))
                        {
                            if (l_Koramar->IsAIEnabled)
                            {
                                switch (m_Modifier)
                                {
                                    case 0:
                                        l_Zoggosh->AI()->Talk(eTalks::TalkZoggosh06);
                                        l_Koramar->m_Events.AddEvent(new aftergrimrail_event(l_Koramar, 1), l_Koramar->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    case 1:
                                        l_Koramar->AI()->Talk(eTalks::TalkKoramar07);
                                        break;
                                    default:
                                        break;
                                }
                            }
                        }
                    }
                }
            }

            return true;
        }
    private:
        Unit* m_Obj;
        int m_Modifier;
};

Position const g_TrainSpawnPos = { 6617.87f, -1200.69f, 9.801f, 3.089053f };
Position const g_TrainMovePos  = { 6407.73f, -1200.30f, 9.800f, 3.126752f };

/// Darkmoon Gazer - 90283
class iron_docks_grimrail_spawning_trigger : public CreatureScript
{
    public:
        iron_docks_grimrail_spawning_trigger() : CreatureScript("iron_docks_grimrail_spawning_trigger") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_CanEvent = false;
            }

            bool m_CanEvent;

            uint64 m_TrainGuid;
            uint64 m_MakoggGuid;
            uint64 m_NoxGuid;
            uint64 m_DuguruGuid;

            void Reset() override
            {
                me->setFaction(35);
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(p_Who, 20.0f) && !m_CanEvent)
                {
                    m_CanEvent = true;
                    events.ScheduleEvent(eEvents::EventBegin, 1 * TimeConstants::IN_MILLISECONDS);
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                events.Update(p_Diff);

                switch (events.ExecuteEvent())
                {
                    case eEvents::EventBegin:
                    {
                        if (Creature* l_Train = me->SummonCreature(eCreatures::Train, g_TrainSpawnPos, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                        {
                            l_Train->SetReactState(ReactStates::REACT_PASSIVE);
                            l_Train->setFaction(16);
                            l_Train->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
                            l_Train->SetSpeed(UnitMoveType::MOVE_RUN, 20.0f, true);
                            l_Train->SetSpeed(UnitMoveType::MOVE_WALK, 20.0f, true);
                            l_Train->GetMotionMaster()->MovePoint(0, g_TrainMovePos.GetPositionX(), g_TrainMovePos.GetPositionY(), g_TrainMovePos.GetPositionZ());

                            m_TrainGuid = l_Train->GetGUID();
                        }

                        events.ScheduleEvent(eEvents::EventProceed1, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventProceed1:
                    {
                        if (Creature* l_Train = Creature::GetCreature(*me, m_TrainGuid))
                        {
                            if (Creature* l_Makogg = l_Train->SummonCreature(eIronDocksCreatures::NpcNox, *l_Train, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                                m_NoxGuid = l_Makogg->GetGUID();
                            if (Creature* l_Augur = l_Train->SummonCreature(eIronDocksCreatures::NpcDuguru, *l_Train, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                                m_DuguruGuid = l_Augur->GetGUID();
                            if (Creature* l_Makogg = l_Train->SummonCreature(eIronDocksCreatures::NpcMakogg, *l_Train, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                                m_MakoggGuid = l_Makogg->GetGUID();
                        }

                        events.ScheduleEvent(eEvents::EventProceed2, 11 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventProceed2:
                    {
                        Creature* l_Makogg = Creature::GetCreature(*me, m_MakoggGuid);
                        Creature* l_Duguru = Creature::GetCreature(*me, m_DuguruGuid);
                        Creature* l_Nox = Creature::GetCreature(*me, m_NoxGuid);
                        if (l_Makogg && l_Duguru && l_Nox)
                        {
                            l_Makogg->GetMotionMaster()->MoveJump(6508.25f, -1127.71f, 4.958f, 12.0f, 8.0f, 10.0f);
                            l_Duguru->GetMotionMaster()->MoveJump(6512.83f, -1129.69f, 4.958f, 12.0f, 8.0f, 10.0f);
                            l_Nox->GetMotionMaster()->MoveJump(6504.74f, -1131.18f, 4.958f, 12.0f, 8.0f, 10.0f);

                            l_Makogg->SetHomePosition(6508.25f, -1127.71f, 4.958f, 1.614914f);
                            l_Duguru->SetHomePosition(6512.83f, -1129.69f, 4.958f, 1.614914f);
                            l_Nox->SetHomePosition(6504.74f, -1131.18f, 4.958f, 1.614914f);
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Makogg Emberblade - 80805
/// Makogg Emberblade - 86231
class boss_grimrail_makogg : public CreatureScript
{
    public:
        boss_grimrail_makogg() : CreatureScript("boss_grimrail_makogg") { }

        struct boss_grimrail_makoggAI : public BossAI
        {
            boss_grimrail_makoggAI(Creature* p_Creature) : BossAI(p_Creature, eIronDocksDatas::DataGrimrailMakogg)
            {
                m_Instance = me->GetInstanceScript();
                m_Intro = false;
            }

            InstanceScript* m_Instance;
            bool m_Intro;

            void Reset() override
            {
                _Reset();     
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                _EnterCombat();
                events.ScheduleEvent(eEvents::EventLavaSweep, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eEvents::EventFlamingSlash, urand(9 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                _JustDied();
                Talk(eYells::SayDeathMakogg);

                if (m_Instance != nullptr)
                {
                    if (Creature* l_Nox = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataGrimrailNoxx)))
                    {
                        if (Creature* l_Duguru = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataGrimrailDuguru)))
                        {
                            if (l_Duguru->isDead() && l_Nox->isDead())
                            {
                                m_Instance->SetBossState(eIronDocksDatas::DataGrimrail, EncounterState::DONE);
                                l_Duguru->m_Events.AddEvent(new aftergrimrail_event(l_Duguru, 0), l_Duguru->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
                            }
                            else
                                me->SetLootRecipient(nullptr);
                        }
                    }
                }
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(p_Who, 35.0f) && !m_Intro)
                {
                    m_Intro = true;
                    Talk(eYells::SayIntroMakogg);

                    if (Creature* l_Skulloc = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataSkulloc)))
                    {
                        if (l_Skulloc->GetVehicleKit() && l_Skulloc->GetVehicleKit()->GetPassenger(0))
                        {
                            if (Creature* l_Turret = l_Skulloc->GetVehicleKit()->GetPassenger(0)->ToCreature())
                                l_Turret->m_Events.KillAllEvents(true); ///< Stops bombardment
                        }
                    }

                    me->m_Events.AddEvent(new beforegrimrail_event(me, 0), me->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
                }
            }

            void JustReachedHome() override
            {
                if (Creature* l_Nox = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataGrimrailNoxx)))
                {
                    if (Creature* l_Duguru = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataGrimrailDuguru)))
                    {
                        l_Nox->Respawn();

                        if (l_Nox->IsAIEnabled)
                            l_Nox->GetAI()->Reset();

                        l_Duguru->Respawn();

                        if (l_Duguru->IsAIEnabled)
                            l_Duguru->GetAI()->Reset();

                        m_Instance->SetBossState(eIronDocksDatas::DataGrimrailMakogg, EncounterState::FAIL);
                    }
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (events.ExecuteEvent())
                {
                    case eEvents::EventFlamingSlash:
                    {
                        me->CastSpell(me, eSpells::SpellFlamingSlashDummy);
                        events.ScheduleEvent(eEvents::EventFlamingSlash, urand(9 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                        break;
                    }
                    case eEvents::EventLavaSweep:
                    {
                        Talk(eYells::SaySpell01Makogg);

                        for (uint8 l_I = 0; l_I <= urand(2, 3); l_I++)
                            me->SummonCreature(TriggerLavaSweep, *me, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 8 * TimeConstants::IN_MILLISECONDS);

                        events.ScheduleEvent(eEvents::EventLavaSweep, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
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
            return new boss_grimrail_makoggAI(p_Creature);
        }
};

/// TRIGGER_LAVA_SWEEP - 95353
class iron_docks_lava_sweep_trigger : public CreatureScript
{
    public:
        iron_docks_lava_sweep_trigger() : CreatureScript("iron_docks_lava_sweep_trigger") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                me->setFaction(16);
            }

            uint32 m_VisualTimer;

            void Reset() override
            {
                me->AddAura(eSpells::SpellLavaSweepVisual, me);
                me->SetSpeed(UnitMoveType::MOVE_RUN, 0.8f, true);

                Position l_Pos;
                me->GetRandomNearPosition(l_Pos, 60.0f);
                me->GetMotionMaster()->MovePoint(0, l_Pos);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (m_VisualTimer <= p_Diff)
                {
                    std::list<Player*> l_PlayerList;
                    me->GetPlayerListInGrid(l_PlayerList, 3.0f);

                    for (Player* l_Iter : l_PlayerList)
                    {
                        me->CastSpell(l_Iter, eSpells::SpellLavaSweepDamage);

                        if (AuraPtr l_SweepAura = l_Iter->GetAura(eSpells::SpellLavaSweepDamage))
                            l_SweepAura->SetDuration(2);
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Flaming Slash - 163665
class spell_flaming_slash_damage_targets : public SpellScriptLoader
{
    public:
        spell_flaming_slash_damage_targets() : SpellScriptLoader("spell_flaming_slash_damage_targets") { }

        class iron_docks_spells : public SpellScript
        {
            PrepareSpellScript(iron_docks_spells);

            void HandleAfterCast()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    std::list<Player*> l_PlayerList;
                    l_Caster->GetPlayerListInGrid(l_PlayerList, 100.0f);

                    for (Player* l_Iter : l_PlayerList)
                        l_Caster->CastSpell(l_Caster, eSpells::SpellFlamingSlashDamage);

                    l_Caster->RemoveAura(eSpells::SpellFlamingSlashDummy);
                    l_Caster->RemoveAura(eSpells::SpellFlamingSlashUnkAura);
                }
            }

            void Register() override
            {
                AfterCast += SpellCastFn(iron_docks_spells::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new iron_docks_spells();
        }
};

/// Flaming Slash - 163668
class spell_flaming_slash_damage_target_change : public SpellScriptLoader
{
    public:
        spell_flaming_slash_damage_target_change() : SpellScriptLoader("spell_flaming_slash_damage_target_change") { }

        class iron_docks_spells : public SpellScript
        {
            PrepareSpellScript(iron_docks_spells);

            bool Load() override
            {
                SpellInfo* l_SpellInfo = const_cast<SpellInfo*>(GetSpellInfo());
                l_SpellInfo->Effects[0].TargetA = 0;
                l_SpellInfo->Effects[0].TargetB = Targets::TARGET_UNIT_NEARBY_ENEMY;
                return true;
            }

            void Register() override
            {
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new iron_docks_spells();
        }
};

/// Tainted Blood - 163740
class spell_tainted_blood_damage_target_change : public SpellScriptLoader
{
    public:
        spell_tainted_blood_damage_target_change() : SpellScriptLoader("spell_tainted_blood_damage_target_change") { }

        class iron_docks_spells : public SpellScript
        {
            PrepareSpellScript(iron_docks_spells);

            bool Load() override
            {
                SpellInfo* l_SpellInfo = const_cast<SpellInfo*>(GetSpellInfo());
                l_SpellInfo->Effects[0].TargetB = Targets::TARGET_UNIT_NEARBY_ENEMY;
                return true;
            }

            void Register() override
            {
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new iron_docks_spells();
        }
};

/// Neesa Nox - 80808
class boss_grimrail_noxx : public CreatureScript
{
    public:
        boss_grimrail_noxx() : CreatureScript("boss_grimrail_noxx") { }

        struct boss_grimrail_noxxAI : public BossAI
        {
            boss_grimrail_noxxAI(Creature* p_Creature) : BossAI(p_Creature, eIronDocksDatas::DataGrimrailNoxx)
            {
                m_Instance = me->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            bool m_Intro;

            void Reset() override
            {
                _Reset();
                m_Intro = false;
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(p_Who, 18.0f) && !m_Intro)
                {
                    m_Intro = true;
                    Talk(eYells::SayIntro);
                }
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                _EnterCombat();
    
                events.ScheduleEvent(eEvents::EventOgreTrap, urand(18 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eEvents::EventJumperCables, 15 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eEvents::EventGutShot, 4 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eEvents::EventBombSquad, 30 * TimeConstants::IN_MILLISECONDS);
            }

            void KilledUnit(Unit* p_Who) override
            {
                if (p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eYells::SaySlay);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                Talk(eYells::SayDeath);
                summons.DespawnAll();

                if (m_Instance != nullptr)
                {
                    if (Creature* l_Makogg = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataGrimrailMakogg)))
                    {
                        if (Creature* l_Duguru = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataGrimrailDuguru)))
                        {
                            if (l_Duguru->isDead() && l_Makogg->isDead())
                            {
                                m_Instance->SetBossState(eIronDocksDatas::DataGrimrail, EncounterState::DONE);
                                l_Duguru->m_Events.AddEvent(new aftergrimrail_event(l_Duguru, 0), l_Duguru->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
                            }
                            else
                                me->SetLootRecipient(nullptr);
                        }
                    }
                }
            }

            void JustReachedHome() override
            {
                if (Creature* l_Makogg = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataGrimrailMakogg)))
                {
                    if (Creature* l_Duguru = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataGrimrailDuguru)))
                    {
                        summons.DespawnAll();

                        l_Makogg->Respawn();

                        if (l_Makogg->IsAIEnabled)
                            l_Makogg->GetAI()->Reset();

                        l_Duguru->Respawn();

                        if (l_Duguru->IsAIEnabled)
                            l_Duguru->GetAI()->Reset();

                        m_Instance->SetBossState(eIronDocksDatas::DataGrimrailMakogg, EncounterState::FAIL);
                    }
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;
   
                switch (events.ExecuteEvent())
                {
                    case eEvents::EventGutShot:
                    {
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eSpells::SpellGutShot);
                        events.ScheduleEvent(eEvents::EventGutShot, urand(5 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
                        break;
                    }
                    case eEvents::EventOgreTrap:
                    {
                        Talk(eYells::SaySpell03);

                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        {
                            float l_Range = 5.0f;
                            float l_Angle = 0.0f;
                            float l_Step = (2 * M_PI) / 8;
                            for (uint8 l_I = 0; l_I < 8; ++l_I)
                            {
                                float l_X = l_Target->GetPositionX() + (l_Range * cos(l_Angle));
                                float l_Y = l_Target->GetPositionY() + (l_Range * sin(l_Angle));
                                me->SummonCreature(eCreatures::OgreTrap, l_X, l_Y, l_Target->GetPositionZ(), l_Target->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                                l_Angle += l_Step;
                            }
                        }

                        events.ScheduleEvent(eEvents::EventOgreTrap, urand(18 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
                        break;
                    }
                    case eEvents::EventJumperCables:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0F, true))
                            me->CastSpell(l_Target, eSpells::SpellJumperCables);
                        Talk(eYells::SaySpell01);
                        events.ScheduleEvent(eEvents::EventJumperCables, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventBombSquad:
                    {
                        Position l_Pos;
                        me->GetRandomNearPosition(l_Pos, 30.0f);

                        Talk(eYells::SaySpell02);
                        me->SummonCreature(eCreatures::BombsquadTrigger, l_Pos, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        events.ScheduleEvent(eEvents::EventBombSquad, 30 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new boss_grimrail_noxxAI(p_Creature);
    }
};

/// Ogre Trap - 88758
class iron_docks_ogre_trap : public CreatureScript
{
    public:
        iron_docks_ogre_trap() : CreatureScript("iron_docks_ogre_trap") { }

        struct mob_iron_docksAI : public Scripted_NoMovementAI
        {
            mob_iron_docksAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
            {
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
            }

            bool m_Activated;
            uint32 m_DespawnTimer;

            void Reset() override
            {
                m_Activated = false;
                me->CastSpell(me, eSpells::SpellOgreTrapOpenTeeth);

                m_DespawnTimer = 12 * TimeConstants::IN_MILLISECONDS;
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!m_Activated)
                {
                    std::list<Player*> l_PlayerList;
                    me->GetPlayerListInGrid(l_PlayerList, 1.0f);

                    for (Player* l_Iter : l_PlayerList)
                    {
                        m_Activated = true;
                        me->RemoveAllAuras();
                        me->AddAura(eSpells::SpellOgreTrapClosedTeeth, me);
                        me->AddAura(eSpells::SpellShreddingTendons, l_Iter);
                        me->DespawnOrUnsummon(6 * TimeConstants::IN_MILLISECONDS);
                    }
                }

                if (m_DespawnTimer <= p_Diff)
                {
                    m_DespawnTimer = 50 * TimeConstants::IN_MILLISECONDS;
                    me->DespawnOrUnsummon(6 * TimeConstants::IN_MILLISECONDS);
                }
                else
                    m_DespawnTimer -= p_Diff;
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Bombsquad - 80875
/// Bombsquad - 580875
class iron_docks_bomb_trap : public CreatureScript
{
    public:
        iron_docks_bomb_trap() : CreatureScript("iron_docks_bomb_trap") { }

        struct mob_iron_docksAI : public Scripted_NoMovementAI
        {
            mob_iron_docksAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature) { }

            void Reset() override
            {
                me->setFaction(16);
                me->CastSpell(me, eSpells::SpellBigBoom);
                me->CastSpell(me, eSpells::SpellBigBoom, true);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Lava Wave - 98928 (WTF!)
class iron_docks_flame_wave_restriction : public SpellScriptLoader
{
    public:
        iron_docks_flame_wave_restriction() : SpellScriptLoader("iron_docks_flame_wave_restriction") { }

        class iron_docks_spells : public SpellScript
        {
            PrepareSpellScript(iron_docks_spells);

            bool Load() override
            {
                if (Unit* l_Caster = GetCaster())
                {
                    SpellInfo* l_SpellInfo = const_cast<SpellInfo*>(GetSpellInfo());
                    l_SpellInfo->Effects[0].TargetA = 0;
                    l_SpellInfo->Effects[0].TargetB = 0;
                    l_SpellInfo->Effects[2].TargetA = 0;
                    l_SpellInfo->Effects[2].TargetB = 0;
                    return true;
                }
            }

            SpellCastResult CheckTarget()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->GetMapId() == 1195)
                        return SpellCastResult::SPELL_FAILED_DONT_REPORT;
                    else
                        return SpellCastResult::SPELL_CAST_OK;
                }
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(iron_docks_spells::CheckTarget);
            }
        };
};

/// Ahri'ok Duguru - 80816
class boss_grimrail_duguru : public CreatureScript
{
    public:
        boss_grimrail_duguru() : CreatureScript("boss_grimrail_duguru") { }

        struct boss_grimrail_duguruAI : public BossAI
        {
            boss_grimrail_duguruAI(Creature* p_Creature) : BossAI(p_Creature, eIronDocksDatas::DataGrimrailDuguru)
            {
                m_Instance = me->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            void Reset() override
            {
                _Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                _EnterCombat();

                events.ScheduleEvent(eEvents::EventBloodBolt, 6 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eEvents::EventSanguineSphere, 10 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eEvents::EventTaintedBlood, 14 * TimeConstants::IN_MILLISECONDS);
            }

            void KilledUnit(Unit* p_Who) override
            {
                if (p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eYells::SaySlay);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                _JustDied();

                Talk(eYells::SayDeath);

                if (m_Instance != nullptr)
                {
                    if (Creature* l_Makogg = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataGrimrailMakogg)))
                    {
                        if (Creature* l_Noxx = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataGrimrailNoxx)))
                        {
                            if (l_Noxx->isDead() && l_Makogg->isDead())
                            {
                                m_Instance->SetBossState(eIronDocksDatas::DataGrimrail, EncounterState::DONE);
                                l_Noxx->m_Events.AddEvent(new aftergrimrail_event(l_Noxx, 0), l_Noxx->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
                            }
                            else
                                me->SetLootRecipient(nullptr);
                        }
                    }
                }
            }

            void JustReachedHome() override
            {
                if (Creature* l_Makogg = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataGrimrailMakogg)))
                {
                    if (Creature* l_Nox = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataGrimrailNoxx)))
                    {
                        l_Makogg->Respawn();

                        if (l_Makogg->IsAIEnabled)
                            l_Makogg->GetAI()->Reset();

                        l_Nox->Respawn();

                        if (l_Nox->IsAIEnabled)
                            l_Nox->GetAI()->Reset();

                        m_Instance->SetBossState(eIronDocksDatas::DataGrimrailMakogg, EncounterState::FAIL);
                    }
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (events.ExecuteEvent())
                {                  
                    case eEvents::EventBloodBolt:
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eSpells::SpellBloodBolt);
                        events.ScheduleEvent(eEvents::EventBloodBolt, 6 * TimeConstants::IN_MILLISECONDS + 2500);
                        break;
                    case eEvents::EventSanguineSphere:
                        me->CastSpell(me, eSpells::SpellSanguineSphere, true);
                        events.ScheduleEvent(eEvents::EventSanguineSphere, 16 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventTaintedBlood:
                        if (Unit * l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            me->CastSpell(l_Target, eSpells::SpellTaintedBlood);
                        events.ScheduleEvent(eEvents::EventTaintedBlood, 14 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_grimrail_duguruAI(p_Creature);
        }
};

/// Sanguine Sphere - 163689
class spell_sanguine_sphere : public SpellScriptLoader
{
    public:
        spell_sanguine_sphere() : SpellScriptLoader("spell_sanguine_sphere") { }

        class spell_iron_docks : public SpellScript
        {
            PrepareSpellScript(spell_iron_docks);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    p_Targets.clear();

                    InstanceScript* l_InstanceScript = l_Caster->GetInstanceScript();
                    if (!l_InstanceScript)
                        return;

                    switch (urand(0, 1))
                    {
                        case 0:
                            if (Creature* l_Makogg = l_InstanceScript->instance->GetCreature(l_InstanceScript->GetData64(eIronDocksDatas::DataGrimrailMakogg)))
                                p_Targets.push_back(l_Makogg);
                            break;
                        case 1:
                            if (Creature* l_Nox = l_InstanceScript->instance->GetCreature(l_InstanceScript->GetData64(eIronDocksDatas::DataGrimrailNoxx)))
                                p_Targets.push_back(l_Nox);
                            break;
                        default:
                            break;
                    }
                }
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_iron_docks::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_iron_docks::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_iron_docks();
        }
};

class aura_sanguine_removal : public SpellScriptLoader
{
    public:
        aura_sanguine_removal() : SpellScriptLoader("aura_sanguine_removal") { }

        class iron_docks_auras : public AuraScript
        {
            PrepareAuraScript(iron_docks_auras);

            void OnRemove(constAuraEffectPtr /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Target = GetTarget())
                {
                    l_Target->CastSpell(l_Target, eSpells::SpellAbruptRestoration);
                    l_Target->HealBySpell(l_Target, sSpellMgr->GetSpellInfo(eSpells::SpellAbruptRestoration), int32(l_Target->GetHealth() * 0.15f), true);
                }
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(iron_docks_auras::OnRemove, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new iron_docks_auras();
        }
};

class spell_grimrail_lava_wava_amplitude_fix : public SpellScriptLoader
{
    public:
        spell_grimrail_lava_wava_amplitude_fix() : SpellScriptLoader("spell_grimrail_lava_wava_amplitude_fix") { }

        class irondocks_auras : public AuraScript
        {
            PrepareAuraScript(irondocks_auras);

            bool Load() override
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->GetMapId() == 1195)
                    {
                        SpellInfo* l_SpellInfo = const_cast<SpellInfo*>(GetSpellInfo());
                        l_SpellInfo->Effects[0].Amplitude = 4 * TimeConstants::IN_MILLISECONDS;
                        return true;
                    }
                }

                return true;
            }

            void Register() override
            {
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new irondocks_auras();
        }
};

void AddSC_boss_grimrail()
{
    /// Bosses
    new boss_grimrail_noxx();
    new boss_grimrail_makogg();
    new boss_grimrail_duguru();

    /// Triggers
    new iron_docks_lava_sweep_trigger();
    new iron_docks_ogre_trap();
    new iron_docks_bomb_trap();
    new iron_docks_grimrail_spawning_trigger();

    /// Spells
    new spell_flaming_slash_damage_targets();
    new iron_docks_flame_wave_restriction();
    new spell_flaming_slash_damage_target_change();
    new spell_sanguine_sphere();
    new aura_sanguine_removal();
    new spell_tainted_blood_damage_target_change();
    new spell_grimrail_lava_wava_amplitude_fix();
}