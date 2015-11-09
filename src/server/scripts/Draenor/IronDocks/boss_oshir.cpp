////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "iron_docks.hpp"

enum eSpells
{
    SpellBloodyCorpseExplosion  = 73163
};

enum eEvents
{
    EventRelease
};

enum eAction
{
    ActionRelease = 1
};

enum eTalks
{
    TalkZoggosh11 = 20, ///< Aye! aye! sir! (44058)
    TalkZoggosh08 = 17, ///< Ahm... are you sure you want to let that thing out of it's cage? I don't think we fed it since we've found it. (44055)
    TalkZoggosh07 = 16, ///< Captain they're almost here.. what are we do now!? (44054)
    TalkKoramar09 = 31, ///< I'm not playing these games anymore.. bring in the saber on and we'll be done with it. (46908)
    TalkKoramar11 = 32, ///< Put a muzzle on it (46909)
    TalkKoramar10 = 33  ///< I didn't mean.. the Cat... (46910)
};

enum eCreatures
{
    CreatureWolf        = 89012,
    CreatureRylak       = 89011,
    CreatureGuard       = 83390, 
    /// Dests
    TriggerWolfDest     = 89022,
    TriggerRylakDest    = 89021,
    TriggerAcidPool     = 214746
};

Position const g_NewHomePos = { 6945.15f, -1098.29f, 4.603f, 4.579627f };

Position const g_NpcSpawnPos = { 6924.00f, -1090.61f, 4.604f, 0.102262f };

Position const g_NpcMovePos[3] =
{
    { 6972.43f, -1094.44f, 4.962f, 0.903368f },
    { 6969.16f, -1089.79f, 4.600f, 0.881810f },
    { 6964.44f, -1085.31f, 4.603f, 0.102262f }
}; 

class basicevent_before_oshir : public BasicEvent
{
    public:

        explicit basicevent_before_oshir(Unit* p_Unit, int p_Value) : m_Obj(p_Unit), m_Modifier(p_Value) { }

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
                                        l_Zoggosh->AI()->Talk(eTalks::TalkZoggosh07);
                                        l_Koramar->m_Events.AddEvent(new basicevent_before_oshir(l_Koramar, 1), l_Koramar->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    case 1:
                                        l_Koramar->AI()->Talk(eTalks::TalkKoramar09);
                                        l_Koramar->m_Events.AddEvent(new basicevent_before_oshir(l_Koramar, 2), l_Koramar->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    case 2:
                                        l_Zoggosh->AI()->Talk(eTalks::TalkZoggosh08);
                                        l_Koramar->m_Events.AddEvent(new basicevent_before_oshir(l_Koramar, 3), l_Koramar->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    case 3:
                                        l_Koramar->AI()->Talk(eTalks::TalkKoramar11);
                                        l_Koramar->m_Events.AddEvent(new basicevent_before_oshir(l_Koramar, 4), l_Koramar->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    case 4:
                                        l_Zoggosh->AI()->Talk(eTalks::TalkZoggosh11);
                                        l_Koramar->m_Events.AddEvent(new basicevent_before_oshir(l_Koramar, 5), l_Koramar->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    case 5:
                                        l_Koramar->AI()->Talk(eTalks::TalkKoramar10);
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

class basicevent_cosmetic_oshir : public BasicEvent
{
    public:

        explicit basicevent_cosmetic_oshir(Unit* p_Unit, int p_Value) : m_Obj(p_Unit), m_Modifier(p_Value) { }

        bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/)
        {
            if (InstanceScript* l_Instance = m_Obj->GetInstanceScript())
            {
                if (Creature* l_Oshir = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataOshir)))
                {
                    if (l_Oshir->IsAIEnabled)
                    {
                        switch (m_Modifier)
                        {
                            case 0:
                            {
                                for (uint8 l_I = 0; l_I < 3; l_I++)
                                {
                                    if (Creature* l_Guard = l_Oshir->SummonCreature(eCreatures::CreatureGuard, g_NpcSpawnPos, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                                    {
                                        l_Guard->GetMotionMaster()->MovePoint(0, g_NpcMovePos[l_I]);
                                        l_Guard->setFaction(FriendlyFaction);
                                    }
                                }
                                l_Oshir->m_Events.AddEvent(new basicevent_cosmetic_oshir(l_Oshir, 1), l_Oshir->m_Events.CalculateTime(6 * TimeConstants::IN_MILLISECONDS));
                                break;
                            }
                            case 1:
                            {
                                if (Creature* l_Guard = l_Oshir->FindNearestCreature(eCreatures::CreatureGuard, 100.0f))
                                {
                                    l_Oshir->GetMotionMaster()->MoveJump(l_Guard->GetPositionX(), l_Guard->GetPositionY(), l_Guard->GetPositionZ(), 25.0f, 10.0f, 10.0f);
                                    l_Guard->CastSpell(l_Guard, eSpells::SpellBloodyCorpseExplosion);
                                    l_Oshir->Kill(l_Guard);
                                    l_Guard->DespawnOrUnsummon(8 * TimeConstants::IN_MILLISECONDS);
                                }
                                l_Oshir->m_Events.AddEvent(new basicevent_cosmetic_oshir(l_Oshir, 2), l_Oshir->m_Events.CalculateTime(3 * TimeConstants::IN_MILLISECONDS));
                                break;
                            }
                            case 2:
                            {
                                if (Creature* l_Guard = l_Oshir->FindNearestCreature(eCreatures::CreatureGuard, 100.0f))
                                {

                                    l_Oshir->GetMotionMaster()->MoveJump(*l_Guard, 25.0f, 10.0f, 10.0f);
                                    l_Guard->GetMotionMaster()->MoveRandom(5.0f);
                                    l_Guard->CastSpell(l_Guard, eSpells::SpellBloodyCorpseExplosion);
                                    l_Oshir->Kill(l_Guard);
                                    l_Guard->DespawnOrUnsummon(8 * TimeConstants::IN_MILLISECONDS);
                                }
                                l_Oshir->m_Events.AddEvent(new basicevent_cosmetic_oshir(l_Oshir, 3), l_Oshir->m_Events.CalculateTime(3 * TimeConstants::IN_MILLISECONDS));
                                break;
                            }
                            case 3:
                            {
                                if (Creature* l_Guard = l_Oshir->FindNearestCreature(eCreatures::CreatureGuard, 100.0f))
                                {
                                    l_Guard->GetMotionMaster()->MoveRandom(5.0f);
                                    l_Oshir->GetMotionMaster()->MoveJump(*l_Guard, 25.0f, 10.0f, 10.0f);
                                    l_Guard->DespawnOrUnsummon(8 * TimeConstants::IN_MILLISECONDS);
                                    l_Guard->CastSpell(l_Guard, eSpells::SpellBloodyCorpseExplosion);
                                    l_Oshir->Kill(l_Guard);                              
                                }
                                l_Oshir->m_Events.AddEvent(new basicevent_cosmetic_oshir(l_Oshir, 4), l_Oshir->m_Events.CalculateTime(3 * TimeConstants::IN_MILLISECONDS));
                                break;
                            }
                            case 4:
                            {
                                l_Oshir->SetHomePosition(g_NewHomePos);
                                l_Oshir->GetMotionMaster()->MovePoint(0, g_NewHomePos);
                                l_Oshir->setFaction(HostileFaction);
                                break;
                            }
                            default:
                                break;
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

/// Oshir - 79852
class boss_oshir : public CreatureScript
{
    public:

        boss_oshir() : CreatureScript("boss_oshir") { }

        struct boss_oshirAI : public BossAI
        {
            boss_oshirAI(Creature* p_Creature) : BossAI(p_Creature, eIronDocksDatas::DataOshir)
            {
                m_Intro = false;
                me->setFaction(FriendlyFaction);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_UNK_15 | eUnitFlags::UNIT_FLAG_UNK_6);
            }

            enum eOshirEvents
            {
                EventBreakout = 1,
                EventFeedingFrenzy,
                EventPrimalAssault,
                EventFeedingFrenzyCancel
            };

            enum eOshirSpells
            {
                SpellFeedingAura   = 162424,
                SpellBreakout      = 178124,
                SpellFeedingFrenzy = 162415,
                SpellPrimalAssault = 161256
            };

            bool m_Intro;
            uint32 m_HpPact;
            std::list<uint64> m_WolfDests;
            std::list<uint64> m_RylakDests;

            void Reset() override
            {
                _Reset();
                events.Reset();
                m_WolfDests.clear();
                m_RylakDests.clear();
                m_HpPact = me->GetHealthPct() * 0.95;
                DespawnCreaturesInArea(eCreatures::CreatureWolf, me);
                DespawnCreaturesInArea(eCreatures::CreatureRylak, me);
                me->SetSpeed(UnitMoveType::MOVE_FLIGHT, 10.0f, true);

                std::list<Creature*> l_WolfsTempList;
                std::list<Creature*> l_RylaksTempList;
                me->GetCreatureListWithEntryInGrid(l_WolfsTempList, eCreatures::TriggerWolfDest, 300.0f);
                me->GetCreatureListWithEntryInGrid(l_RylaksTempList, eCreatures::TriggerRylakDest, 300.0f);
                if (!l_WolfsTempList.empty())
                {
                    for (auto l_Itr : l_WolfsTempList)
                    {
                        if (l_Itr->IsAIEnabled)
                        {
                            l_Itr->GetAI()->Reset();
                            m_WolfDests.push_back(l_Itr->GetGUID());
                        }
                    }
                }
                if (!l_RylaksTempList.empty())
                {
                    for (auto l_Iter : l_RylaksTempList)
                    {
                        if (l_Iter->IsAIEnabled)
                        {
                            l_Iter->GetAI()->Reset();
                            m_RylakDests.push_back(l_Iter->GetGUID());
                        }
                    }
                }        
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                _EnterCombat();

                events.ScheduleEvent(eOshirEvents::EventFeedingFrenzy, urand(50 * TimeConstants::IN_MILLISECONDS, 60 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eOshirEvents::EventBreakout, 20 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eOshirEvents::EventPrimalAssault, urand(35 * TimeConstants::IN_MILLISECONDS, 40 * TimeConstants::IN_MILLISECONDS));
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo) override
            {
                if (p_Damage && p_Damage > 0)
                {
                    if (m_HpPact)
                    {
                        if (me->HealthBelowPctDamaged(m_HpPact, p_Damage))
                        {
                            me->CastStop();
                            me->RemoveAllAuras();
                            me->RemoveAura(eOshirSpells::SpellFeedingFrenzy);
                            me->RemoveAura(eOshirSpells::SpellFeedingAura);
                        }
                    }
                }
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                _JustDied();

                DespawnCreaturesInArea(eCreatures::CreatureWolf, me);
                DespawnCreaturesInArea(eCreatures::CreatureRylak, me);
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(p_Who, 50.0f) && !m_Intro)
                {
                    m_Intro = true;
                    me->m_Events.AddEvent(new basicevent_cosmetic_oshir(me, 0), me->m_Events.CalculateTime(2 * TimeConstants::IN_MILLISECONDS));
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING) || me->HasAura(eOshirSpells::SpellFeedingAura))
                    return;

                switch (events.ExecuteEvent())
                {
                case eOshirEvents::EventFeedingFrenzyCancel:
                    {
                        me->RemoveAura(eOshirSpells::SpellFeedingFrenzy);
                        break;
                    }
                case eOshirEvents::EventFeedingFrenzy:
                    {
                        me->RemoveAura(eOshirSpells::SpellFeedingFrenzy);
                        m_HpPact = me->GetHealthPct() * 0.95;

                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            me->CastSpell(l_Target, eOshirSpells::SpellFeedingFrenzy);
                        events.ScheduleEvent(eOshirEvents::EventFeedingFrenzy, urand(50 * TimeConstants::IN_MILLISECONDS, 60 * TimeConstants::IN_MILLISECONDS));
                        events.ScheduleEvent(eOshirEvents::EventFeedingFrenzyCancel, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                case eOshirEvents::EventPrimalAssault:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        {
                            me->CastSpell(l_Target, eOshirSpells::SpellBreakout);
                            me->CastSpell(l_Target, eOshirSpells::SpellPrimalAssault);
                        }
                        events.ScheduleEvent(eOshirEvents::EventPrimalAssault, urand(35 * TimeConstants::IN_MILLISECONDS, 40 * TimeConstants::IN_MILLISECONDS));
                        break;
                    }
                case eOshirEvents::EventBreakout:
                    {
                        me->RemoveAura(eOshirSpells::SpellFeedingAura);

                        switch (urand(0, 1))
                        {
                            case 0: ///< Wolfs
                            {
                                if (m_WolfDests.empty())
                                    return;

                                auto l_Iter = m_WolfDests.begin();
                                std::advance(l_Iter, urand(0, m_WolfDests.size() - 1));
                                if (Creature* l_Wolf = Unit::GetCreature(*me, *l_Iter))
                                {
                                    me->GetMotionMaster()->MoveJump(*l_Wolf, 25.0f, 10.0f, 10.0f);
                                    if (l_Wolf->IsAIEnabled)
                                    {
                                        l_Wolf->GetAI()->DoAction(eAction::ActionRelease);
                                        m_WolfDests.erase(l_Iter);
                                    }
                                }
                                break;
                            }
                            case 1: ///< Rylaks
                            {
                                if (m_RylakDests.empty())
                                    return;

                                auto l_Iter = m_RylakDests.begin();
                                std::advance(l_Iter, urand(0, m_RylakDests.size() - 1));
                                if (Creature* l_Rylak = Unit::GetCreature(*me, *l_Iter))
                                {                                
                                    if (l_Rylak->IsAIEnabled)
                                    {
                                        me->GetMotionMaster()->MoveJump(*l_Rylak, 25.0f, 10.0f, 10.0f);
                                        l_Rylak->GetAI()->DoAction(eAction::ActionRelease);
                                        m_RylakDests.erase(l_Iter);
                                    }
                                }
                                break;
                            }
                            default:
                                break;
                        }
                        events.ScheduleEvent(eOshirEvents::EventBreakout, 20 * TimeConstants::IN_MILLISECONDS);
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
            return new boss_oshirAI(p_Creature);
        }
};

/// Wolf Dest - 89022
class iron_docks_oshir_mob_wolf_dest : public CreatureScript
{
    public:

        iron_docks_oshir_mob_wolf_dest() : CreatureScript("iron_docks_oshir_mob_wolf_dest") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = me->GetInstanceScript();
            }
 
            InstanceScript* m_Instance;
            bool m_Released;

            void Reset() override
            {
                m_Released = false;
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eAction::ActionRelease:
                    {
                        if (Creature* l_Oshir = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataOshir)))
                        {
                                if (!m_Released)
                                {
                                    m_Released = true;
                                    events.ScheduleEvent(eEvents::EventRelease, 5 * TimeConstants::IN_MILLISECONDS);
                                }
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                events.Update(p_Diff);

                switch (events.ExecuteEvent())
                {
                    case eEvents::EventRelease:
                    {
                        if (Creature* l_Oshir = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataOshir)))
                        {
                            std::list<Creature*> l_TempWolfsList;
                            me->GetCreatureListWithEntryInGrid(l_TempWolfsList, eCreatures::CreatureWolf, 2.0f);

                            if (l_TempWolfsList.empty())
                                return;

                            for (auto l_Itr : l_TempWolfsList)
                            {
                                l_Itr->setFaction(HostileFaction);
                                l_Itr->SetReactState(ReactStates::REACT_AGGRESSIVE);
                                if (Player* l_Player = me->FindNearestPlayer(200.0f, true))
                                    l_Itr->Attack(l_Player, true);
                            }
                        }
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
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Rylak Dest - 89021
class iron_docks_oshir_mob_rylak_dest : public CreatureScript
{
    public:

        iron_docks_oshir_mob_rylak_dest() : CreatureScript("iron_docks_oshir_mob_rylak_dest") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = me->GetInstanceScript();
            }
  
            InstanceScript* m_Instance;
            bool m_Released;

            void Reset() override
            {
                m_Released = false;
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eAction::ActionRelease:
                    {
                        if (Creature* l_Oshir = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataOshir)))
                        {
                            if (!m_Released)
                            {
                                m_Released = true;
                                events.ScheduleEvent(eEvents::EventRelease, 5 * TimeConstants::IN_MILLISECONDS);
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                events.Update(p_Diff);

                switch (events.ExecuteEvent())
                {
                    case eEvents::EventRelease:
                    {
                        if (Creature* l_Oshir = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataOshir)))
                        {
                            std::list<Creature*> l_TempRylaksList;
                            me->GetCreatureListWithEntryInGrid(l_TempRylaksList, eCreatures::CreatureRylak, 2.0f);

                            if (l_TempRylaksList.empty())
                                return;

                            for (auto l_Itr : l_TempRylaksList)
                            {
                                l_Itr->setFaction(HostileFaction);
                                l_Itr->SetReactState(ReactStates::REACT_AGGRESSIVE);
                                if (Player* l_Player = me->FindNearestPlayer(200.0f, true))
                                    l_Itr->Attack(l_Player, true);
                            }
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

/// Oshir Event - 90282
class iron_docks_oshir_mob_event : public CreatureScript
{
public:

    iron_docks_oshir_mob_event() : CreatureScript("iron_docks_oshir_mob_event") { }

    struct mob_iron_docksAI : public ScriptedAI
    {
        mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_CanEvent = false;
        }

        bool m_CanEvent;

        void MoveInLineOfSight(Unit* p_Who) override
        {
            if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(p_Who, 40.0) && !m_CanEvent)
            {
                m_CanEvent = true;
                me->m_Events.AddEvent(new basicevent_before_oshir(me, 0), me->m_Events.CalculateTime(2 * TimeConstants::IN_MILLISECONDS));
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new mob_iron_docksAI(p_Creature);
    }
};

/// Rylak Skyterror - 89011
class iron_docks_oshir_mob_rylak : public CreatureScript
{
    public:

        iron_docks_oshir_mob_rylak() : CreatureScript("iron_docks_oshir_mob_rylak") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = me->GetInstanceScript();
                m_First = false;
            }

            enum eAcidSpitSpells
            {
                /// Rylak
                SpellAcidicCast = 171529,
                SpellAcidPool   = 171533
            };

            bool m_First;
            uint32 m_AcidTimer;
            InstanceScript* m_Instance;

            void Reset() override
            {
                if (!m_First)
                {
                    m_First = true;
                    me->SetReactState(ReactStates::REACT_PASSIVE);
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
                }

                m_AcidTimer = urand(11 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                if (m_AcidTimer <= p_Diff)
                {
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                    {
                        // has to get recoded
                    }
                    m_AcidTimer = urand(11 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS);
                }
                else
                    m_AcidTimer -= p_Diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Wolf (Loup Vorace) - 89012
class iron_docks_oshir_mob_wolf : public CreatureScript
{
    public:

        iron_docks_oshir_mob_wolf() : CreatureScript("iron_docks_oshir_mob_wolf") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = me->GetInstanceScript();
            }

            enum eWolfSpells
            {
                SpellStrengthOfThePack = 178149
            };

            uint32 m_Timer;
            InstanceScript* m_Instance;

            void Reset() override
            {
                m_Timer = 1 * TimeConstants::IN_MILLISECONDS;
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;
                
                if (m_Timer <= p_Diff) 
                {
                    if (!me->HasAura(eWolfSpells::SpellStrengthOfThePack))
                        me->AddAura(eWolfSpells::SpellStrengthOfThePack, me);

                    std::list<Creature*> l_WolfTempoList;
                    me->GetCreatureListWithEntryInGrid(l_WolfTempoList, eCreatures::CreatureWolf, 5.0f);
                    if (!l_WolfTempoList.empty())
                    {
                        if (AuraPtr l_Aura = me->GetAura(eWolfSpells::SpellStrengthOfThePack))
                        {
                            l_Aura->SetStackAmount(l_WolfTempoList.size());
                        }
                    }

                    m_Timer = 1 * TimeConstants::IN_MILLISECONDS;
                }
                else
                    m_Timer -= p_Diff;
                   
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Acid Spit - 
class iron_docks_oshir_mob_acid_spit : public CreatureScript
{
    public:

        iron_docks_oshir_mob_acid_spit() : CreatureScript("iron_docks_oshir_mob_acid_spit") { }      

        struct mob_iron_docksAI : public Scripted_NoMovementAI
        {
            mob_iron_docksAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
            {
                m_Instance = me->GetInstanceScript();
            }

            enum eAcidSpitSpells
            {
                /// Rylak
                SpellAcidicCast = 171529,
                SpellAcidPool   = 171533
            };

            uint32 m_Timer;
            InstanceScript* m_Instance;

            void Reset() override
            {
                me->setFaction(HostileFaction);
                m_Timer = 1 * TimeConstants::IN_MILLISECONDS;         
                me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (m_Timer <= p_Diff)
                {
                    std::list<Player*> l_PlayerList;
                    JadeCore::AnyPlayerInObjectRangeCheck l_Check(me, 2.0f);
                    JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> l_Searcher(me, l_PlayerList, l_Check);
                    me->VisitNearbyObject(2.0f, l_Searcher);
                    if (!l_PlayerList.empty())
                    {
                        for (auto l_Itr : l_PlayerList)
                        {
                            if (!l_Itr->HasAura(eAcidSpitSpells::SpellAcidPool))
                            {
                                l_Itr->CastSpell(l_Itr, eAcidSpitSpells::SpellAcidPool);
                            }
                            else if (AuraPtr l_Aura = l_Itr->GetAura(eAcidSpitSpells::SpellAcidPool))
                            {                              
                                l_Aura->SetDuration(1);
                            }                  
                        }
                    }

                    m_Timer = 1 * TimeConstants::IN_MILLISECONDS;
                }
                else
                    m_Timer -= p_Diff;
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};

void AddSC_boss_oshir()
{
    /// Boss
    new boss_oshir();
    /// Mobs
    new iron_docks_oshir_mob_rylak();
    new iron_docks_oshir_mob_wolf();
    new iron_docks_oshir_mob_acid_spit();
    /// Triggers
    new iron_docks_oshir_mob_rylak_dest();
    new iron_docks_oshir_mob_wolf_dest();
    new iron_docks_oshir_mob_event();
}