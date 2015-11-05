////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "iron_docks.hpp"

enum eTalks
{
    /// Noxx
    SayIntro            = 50,   ///< Is there killing to be done? Let me have 'em! (46128)
    SaySlay             = 3,    ///< Tick.. tick.. tick.. BOOM! (46129)
    SayDeath            = 4,    ///< I'm hit... (46127)
    SaySpell01          = 1,    ///< Now you ain't so tall no more! (46130)
    SaySpell02          = 2,    ///< You better step back. I'm calling in the bomb squad! (46131)
    SaySpell03          = 5,    ///< Time is money friend, and it looks like you out of both! (46132)
    SayIntroMakogg      = 60,   ///<  Hah! (45800)
    SayDeathMakogg      = 61,   ///<  This.. is.. far, from over.. (45799)
    SaySpell01Makogg    = 62,   ///<  You will burn! (45801)
    TalkZoggosh05       = 14,   ///< Sir.. he's out of ammunition. I'm reloading.. I'm reloading!! (44052)
    TalkKoramar05       = 27,   ///< What is wrong with you?! keep firing you all! (46904)
    TalkKoramar06       = 28,   ///< Arghh.. forget it, they'll never make it pass the tracks.. (unknown message) (46905)
    TalkZoggosh06       = 15,   ///< Sir.. half of our army has been taken out. Don't you think we should.. (44053)
    TalkKoramar07       = 29    ///< That was an insignificant loss... let them exhaust themselves playing against our weak.. against those who're unworthy of serving in Black Hands army. That's all what Orc understand(46906)
};

enum eSpells
{ 
    /// Bombsquad
    SpellBigBoom        = 163379
};

enum eEvents
{
    /// Train Event
    EventBegin = 1,
    EventProceed1,
    EventProceed2
};

enum eCreatures
{
    CreatureLavaSweep           = 95353,
    CreatureOgreTrap            = 88758,
    CreatureBombsquad           = 80875,
    CreatureTrain               = 83673
};

Position const g_TrainSpawnPos = { 6617.87f, -1200.69f, 9.801f, 3.089053f };
Position const g_TrainMovePos = { 6407.73f, -1200.30f, 9.800f, 3.126752f };

class basicevent_beforegrimrail : public BasicEvent
{
    public:

        explicit basicevent_beforegrimrail(Unit* p_Unit, int p_Value) : m_Obj(p_Unit), m_Modifier(p_Value) { }

        bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/)
        {
            if (InstanceScript* l_Instance = m_Obj->GetInstanceScript())
            {
                if (Creature* l_Zoggosh = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataZuggosh)))
                {
                    if (Creature* l_Koramar = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataKoramar)))
                    {
                        if (l_Zoggosh->IsAIEnabled && l_Koramar->IsAIEnabled)
                        {
                            switch (m_Modifier)
                            {
                            case 0:
                                l_Koramar->AI()->Talk(eTalks::TalkKoramar05);
                                l_Koramar->m_Events.AddEvent(new basicevent_beforegrimrail(l_Koramar, 1), l_Koramar->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
                                break;
                            case 1:
                                l_Zoggosh->AI()->Talk(eTalks::TalkZoggosh05);
                                l_Koramar->m_Events.AddEvent(new basicevent_beforegrimrail(l_Koramar, 2), l_Koramar->m_Events.CalculateTime(10 * TimeConstants::IN_MILLISECONDS));
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

            return true;
        }
    private:

        Unit* m_Obj;
        int m_Modifier;
};

class basicevent_aftergrimrail : public BasicEvent
{
    public:

        explicit basicevent_aftergrimrail(Unit* p_Unit, int p_Value) : m_Obj(p_Unit), m_Modifier(p_Value) { }

        bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/)
        {
            if (InstanceScript* l_Instance = m_Obj->GetInstanceScript())
            {
                if (Creature* l_Zoggosh = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataZuggosh)))
                {
                    if (Creature* l_Koramar = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataKoramar)))
                    {
                        if (l_Koramar->IsAIEnabled && l_Zoggosh->IsAIEnabled)
                        {
                            switch (m_Modifier)
                            {
                            case 0:
                                l_Zoggosh->AI()->Talk(eTalks::TalkZoggosh06);
                                l_Koramar->m_Events.AddEvent(new basicevent_aftergrimrail(l_Koramar, 1), l_Koramar->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
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

            return true;
        }
    private:

        Unit* m_Obj;
        int m_Modifier;
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

        enum eDuguruSpells
        {
            SpellBloodBolt          = 165122,
            SpellSanguineSphere     = 163689,
            SpellAbruptRestoration  = 163705,
            SpellTaintedBlood       = 163740
        };

        enum eDuguruEvents
        {
            EventBloodBolt,
            EventSanguineSphere,
            EventTaintedBlood
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            _Reset();
            events.Reset();
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            _EnterCombat();
            events.ScheduleEvent(eDuguruEvents::EventBloodBolt, 6 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eDuguruEvents::EventSanguineSphere, 10 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eDuguruEvents::EventTaintedBlood, 14 * TimeConstants::IN_MILLISECONDS);
        }

        void KilledUnit(Unit* p_Who) override
        {
            if (p_Who && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
                Talk(eTalks::SaySlay);
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            _JustDied();
            Talk(eTalks::SayDeath);

            // Just died
        }

        void JustReachedHome() override
        {
            // Just reached home.
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
                case eDuguruEvents::EventBloodBolt:
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eDuguruSpells::SpellBloodBolt);
                        events.ScheduleEvent(eDuguruEvents::EventBloodBolt, 6 * TimeConstants::IN_MILLISECONDS);
                        break;
                case eDuguruEvents::EventSanguineSphere:
                        me->CastSpell(me, eDuguruSpells::SpellSanguineSphere, true);
                        events.ScheduleEvent(eDuguruEvents::EventSanguineSphere, 16 * TimeConstants::IN_MILLISECONDS);
                        break;
                case eDuguruEvents::EventTaintedBlood:
                        if (Unit * l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            me->CastSpell(l_Target, eDuguruSpells::SpellTaintedBlood);
                        events.ScheduleEvent(eDuguruEvents::EventTaintedBlood, 14 * TimeConstants::IN_MILLISECONDS);
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

            enum eMakoggSpells
            {
                SpellFlamingSlashDummy   = 163665,
                SpellFlamingSlashUnkAura = 163669,
                SpellFlamingSlashDamage  = 163668,
                SpellLavaSweepDamage     = 165152
            };

            enum eMakoggEvents
            {
                /// Makogg
                EventFlamingSlash = 1,
                EventLavaSweep        
            };

            InstanceScript* m_Instance;
            bool m_Intro;

            void Reset() override
            {
                _Reset();     
                events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                _EnterCombat();
                events.ScheduleEvent(eMakoggEvents::EventLavaSweep, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eMakoggEvents::EventFlamingSlash, urand(9 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                _JustDied();
                Talk(eTalks::SayDeathMakogg);

                // just died
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(p_Who, 35.0f) && !m_Intro)
                {
                    m_Intro = true;
                    Talk(eTalks::SayIntroMakogg);
                     
                    // Responsible to stop the bombardment event? maybe?
                }
            }

            void JustReachedHome() override
            {
                if (m_Instance != nullptr)
                {
                    // Responsible for respawning them if they died.
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
                    case eMakoggEvents::EventFlamingSlash:
                        {
                            me->CastSpell(me, eMakoggSpells::SpellFlamingSlashDummy);
                            events.ScheduleEvent(eMakoggEvents::EventFlamingSlash, urand(9 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                            break;
                        }
                    case eMakoggEvents::EventLavaSweep:
                        {
                            Talk(eTalks::SaySpell01Makogg);
                            // Responsible for lava sweep summoning.
                            events.ScheduleEvent(eMakoggEvents::EventLavaSweep, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
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
                m_Intro = false;
            }

            enum eNoxxSpells
            {
                /// Noxx
                SpellGutShot             = 163334,
                SpellOgreTrapOpenTeeth   = 177391,
                SpellOgreTrapClosedTeeth = 177396,
                SpellShreddingTendons    = 163276,
                SpellJumperCables        = 163376
            };

            enum eNoxxEvents
            {
                /// Noxx
                EventGutShot = 1,
                EventOgreTrap,
                EventJumperCables,
                EventBombSquad
            };

            InstanceScript* m_Instance;
            bool m_Intro;

            void Reset() override
            {
                events.Reset();
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(p_Who, 18.0f) && !m_Intro)
                {
                    m_Intro = true;
                    Talk(eTalks::SayIntro);
                }
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                events.ScheduleEvent(eNoxxEvents::EventOgreTrap, urand(18 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eNoxxEvents::EventJumperCables, 15 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eNoxxEvents::EventGutShot, 4 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eNoxxEvents::EventBombSquad, 30 * TimeConstants::IN_MILLISECONDS);
            }

            void KilledUnit(Unit* p_Who) override
            {
                if (p_Who && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalks::SaySlay);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                Talk(eTalks::SayDeath);
                summons.DespawnAll();

                 // Upon death
            }

            void JustReachedHome() override
            {
                // Upon respawn
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
                    case eNoxxEvents::EventGutShot:
                        {
                            if (Unit* l_Target = me->getVictim())
                                me->CastSpell(l_Target, eNoxxSpells::SpellGutShot);
                            events.ScheduleEvent(eNoxxEvents::EventGutShot, urand(5 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
                            break;
                        }
                    case eNoxxEvents::EventOgreTrap:
                        {                  
                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            {
                                float l_Range = 5.0f;
                                float l_Angle = 0.0f;
                                float l_Step = (2 * M_PI) / 8;
                                for (uint8 l_I = 0; l_I < 8; ++l_I)
                                {
                                    float l_X = l_Target->GetPositionX() + (l_Range * cos(l_Angle));
                                    float l_Y = l_Target->GetPositionY() + (l_Range * sin(l_Angle));
                                    me->SummonCreature(eCreatures::CreatureOgreTrap, l_X, l_Y, l_Target->GetPositionZ(), l_Target->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                                    l_Angle += l_Step;
                                }
                            }
                            Talk(eTalks::SaySpell03);
                            events.ScheduleEvent(eNoxxEvents::EventOgreTrap, urand(18 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
                            break;
                        }
                    case eNoxxEvents::EventJumperCables:
                        {                
                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0F, true))
                                me->CastSpell(l_Target, eNoxxSpells::SpellJumperCables);
                            Talk(eTalks::SaySpell01);
                            events.ScheduleEvent(eNoxxEvents::EventJumperCables, 15 * TimeConstants::IN_MILLISECONDS);
                            break;
                        }
                    case eNoxxEvents::EventBombSquad:
                        {
                            Position l_Pos;
                            Talk(eTalks::SaySpell02);
                            me->GetRandomNearPosition(l_Pos, 30.0f);                
                        
                            // Responsible for bombsquad summoning
                            events.ScheduleEvent(eNoxxEvents::EventBombSquad, 30 * TimeConstants::IN_MILLISECONDS);
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
class iron_docks_grimrail_mob_ogre_trap : public CreatureScript
{
    public:
        iron_docks_grimrail_mob_ogre_trap() : CreatureScript("iron_docks_grimrail_mob_ogre_trap") { }

        struct iron_docks_grimrail_mob_ogre_trapAI : public Scripted_NoMovementAI
        {
            iron_docks_grimrail_mob_ogre_trapAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
            {           
                m_Instance = me->GetInstanceScript();
            }

            enum eOgreTrapSpells
            {
                SpellOgreTrapOpenTeeth   = 177391,
                SpellOgreTrapClosedTeeth = 177396,
                SpellShreddingTendons    = 163276
            };

            InstanceScript* m_Instance;
            bool m_Activated;
            uint32 m_DespawnTimer;

            void Reset() override
            {
                events.Reset();
                m_Activated = false;
                me->setFaction(HostileFaction);
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->CastSpell(me, eOgreTrapSpells::SpellOgreTrapOpenTeeth);
                me->DespawnOrUnsummon(58 * TimeConstants::IN_MILLISECONDS);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);          
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!m_Activated)
                {
                    if (Player* l_Nearest = me->FindNearestPlayer(1.12f, true))
                    {
                        m_Activated = true;
                        me->RemoveAllAuras();
                        me->AddAura(eOgreTrapSpells::SpellOgreTrapClosedTeeth, me);
                        me->AddAura(eOgreTrapSpells::SpellShreddingTendons, l_Nearest);
                        me->DespawnOrUnsummon(6 * TimeConstants::IN_MILLISECONDS);
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new iron_docks_grimrail_mob_ogre_trapAI(p_Creature);
        }
};

/// Sanguine Sphere - 163689
class iron_docks_grimrail_spell_sanguine_sphere : public SpellScriptLoader
{
    public:
        iron_docks_grimrail_spell_sanguine_sphere() : SpellScriptLoader("iron_docks_grimrail_spell_sanguine_sphere") { }

        enum eSanguineSphereSpells
        {
            SpellSanguineSphere    = 163689,
            SpellAbruptRestoration = 163705,
            SpellTaintedBlood      = 163740
        };

        class iron_docks_grimrail_spell_sanguine_sphere_SpellScript : public SpellScript
        {
            PrepareSpellScript(iron_docks_grimrail_spell_sanguine_sphere_SpellScript);

      
            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    p_Targets.clear();

                    if (InstanceScript* l_InstanceScript = l_Caster->GetInstanceScript())
                    {
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
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(iron_docks_grimrail_spell_sanguine_sphere_SpellScript::CorrectTargets, SpellEffIndex::EFFECT_0, Targets::TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new iron_docks_grimrail_spell_sanguine_sphere_SpellScript();
        }

        class iron_docks_grimrail_spell_sanguine_sphere_AuraScript : public AuraScript
        {
            PrepareAuraScript(iron_docks_grimrail_spell_sanguine_sphere_AuraScript);

            void OnRemove(constAuraEffectPtr /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Target = GetTarget())
                {
                    l_Target->CastSpell(l_Target, eSanguineSphereSpells::SpellAbruptRestoration);
                    l_Target->HealBySpell(l_Target, sSpellMgr->GetSpellInfo(eSanguineSphereSpells::SpellAbruptRestoration), int32(l_Target->GetHealth() * 0.15f), true);
                }
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(iron_docks_grimrail_spell_sanguine_sphere_AuraScript::OnRemove, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new iron_docks_grimrail_spell_sanguine_sphere_AuraScript();
        }
};


void AddSC_boss_grimrail()
{
    /// Bosses
    new boss_grimrail_noxx();
    new boss_grimrail_makogg();
    new boss_grimrail_duguru();
    // Spells
    new iron_docks_grimrail_spell_sanguine_sphere();
    new iron_docks_grimrail_mob_ogre_trap();
}