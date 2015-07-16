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
    SpellFeedingAura            = 162424,
    /// Boss
    SpellBreakout               = 178124,
    SpellFeedingFrenzy          = 162415,
    SpellPrimalAssault          = 161256,
    /// MisC
    SpellBloodyCorpseExplosion  = 73163,
    SpellStrengthOfThePack      = 171114,
    /// Rylak
    SpellAcidicCast             = 171529,
    SpellAcidPool               = 171533
};

enum eEvents
{
    EventBreakout = 900,
    EventFeedingFrenzy,
    EventPrimalAssault,
    EventRelease,
    EventFeedingFrenzyCancel
};

enum eAction
{
    ActionRelease = 424
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

class beforeoshir_event : public BasicEvent
{
    public:
        explicit beforeoshir_event(Unit* p_Unit, int p_Value) : m_Obj(p_Unit), m_Modifier(p_Value) { }

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
                            if (l_Zoggosh->IsAIEnabled && l_Koramar->IsAIEnabled)
                            {
                                switch (m_Modifier)
                                {
                                    case 0:
                                        l_Zoggosh->AI()->Talk(eTalks::TalkZoggosh07);
                                        l_Koramar->m_Events.AddEvent(new beforeoshir_event(l_Koramar, 1), l_Koramar->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    case 1:
                                        l_Koramar->AI()->Talk(eTalks::TalkKoramar09);
                                        l_Koramar->m_Events.AddEvent(new beforeoshir_event(l_Koramar, 2), l_Koramar->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    case 2:
                                        l_Zoggosh->AI()->Talk(eTalks::TalkZoggosh08);
                                        l_Koramar->m_Events.AddEvent(new beforeoshir_event(l_Koramar, 3), l_Koramar->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    case 3:
                                        l_Koramar->AI()->Talk(eTalks::TalkKoramar11);
                                        l_Koramar->m_Events.AddEvent(new beforeoshir_event(l_Koramar, 4), l_Koramar->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    case 4:
                                        l_Zoggosh->AI()->Talk(eTalks::TalkZoggosh11);
                                        l_Koramar->m_Events.AddEvent(new beforeoshir_event(l_Koramar, 5), l_Koramar->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
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
            }
            return true;
        }
    private:
        Unit* m_Obj;
        int m_Modifier;
};

class cosmeticaloshir_event : public BasicEvent
{
    public:
        explicit cosmeticaloshir_event(Unit* p_Unit, int p_Value) : m_Obj(p_Unit), m_Modifier(p_Value) { }

        bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/)
        {
            if (InstanceScript* l_Instance = m_Obj->GetInstanceScript())
            {
                if (Creature* l_Oshir = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataOshir)))
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
                                    l_Guard->setFaction(35);
                                }
                            }

                            l_Oshir->m_Events.AddEvent(new cosmeticaloshir_event(l_Oshir, 1), l_Oshir->m_Events.CalculateTime(6 * TimeConstants::IN_MILLISECONDS));
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

                            l_Oshir->m_Events.AddEvent(new cosmeticaloshir_event(l_Oshir, 2), l_Oshir->m_Events.CalculateTime(3 * TimeConstants::IN_MILLISECONDS));
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

                            l_Oshir->m_Events.AddEvent(new cosmeticaloshir_event(l_Oshir, 3), l_Oshir->m_Events.CalculateTime(3 * TimeConstants::IN_MILLISECONDS));
                            break;
                        }
                        case 3:
                        {
                            if (Creature* l_Guard = l_Oshir->FindNearestCreature(eCreatures::CreatureGuard, 100.0f))
                            {
                                l_Guard->GetMotionMaster()->MoveRandom(5.0f);
                                l_Oshir->GetMotionMaster()->MoveJump(*l_Guard, 25.0f, 10.0f, 10.0f);

                                l_Guard->CastSpell(l_Guard, eSpells::SpellBloodyCorpseExplosion);
                                l_Oshir->Kill(l_Guard);
                                l_Guard->DespawnOrUnsummon(8 * TimeConstants::IN_MILLISECONDS);
                            }

                            l_Oshir->m_Events.AddEvent(new cosmeticaloshir_event(l_Oshir, 4), l_Oshir->m_Events.CalculateTime(3 * TimeConstants::IN_MILLISECONDS));
                            break;
                        }
                        case 4:
                        {
                            l_Oshir->SetHomePosition(g_NewHomePos);
                            l_Oshir->GetMotionMaster()->MovePoint(0, g_NewHomePos);
                            l_Oshir->setFaction(16);
                            break;
                        }
                        default:
                            break;
                    }
                }
            }
            return true;
        }
    private:
        Unit* m_Obj;
        int m_Modifier;
};

/// Oshir Event - 90282
class iron_docks_trigger_stand_fourth_event : public CreatureScript
{
    public:
        iron_docks_trigger_stand_fourth_event() : CreatureScript("iron_docks_trigger_stand_fourth_event") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_CanEvent = false;
            }

            bool m_CanEvent;

            void Reset() override
            {
                me->setFaction(35);
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(p_Who, 40.0) && !m_CanEvent)
                {
                    m_CanEvent = true;
                    me->m_Events.AddEvent(new beforeoshir_event(me, 0), me->m_Events.CalculateTime(2 * TimeConstants::IN_MILLISECONDS));
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
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
                me->setFaction(35);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_UNK_15);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_UNK_6);
            }

            bool m_Intro;
            std::list<uint64> m_WolfDests;
            std::list<uint64> m_RylakDests;
            int32 m_HPPact;

            void Reset() override
            {
                _Reset();
                m_WolfDests.clear();
                m_RylakDests.clear();

                m_HPPact = me->GetHealthPct() * 0.95;
                DespawnCreaturesInArea(eCreatures::CreatureWolf, me);
                DespawnCreaturesInArea(eCreatures::CreatureRylak, me);

                std::list<Creature*> l_Wolfs;
                std::list<Creature*> l_Rylaks;

                me->GetCreatureListWithEntryInGrid(l_Wolfs, eCreatures::TriggerWolfDest, 300.0f);
                me->GetCreatureListWithEntryInGrid(l_Rylaks, eCreatures::TriggerRylakDest, 300.0f);

                for (Creature* l_Iter : l_Wolfs)
                {
                    if (l_Iter->IsAIEnabled)
                        l_Iter->GetAI()->Reset();
                    m_WolfDests.push_back(l_Iter->GetGUID());
                }

                for (Creature* l_Iter : l_Rylaks)
                {
                    if (l_Iter->IsAIEnabled)
                        l_Iter->GetAI()->Reset();
                    m_RylakDests.push_back(l_Iter->GetGUID());
                }

                me->SetSpeed(UnitMoveType::MOVE_FLIGHT, 10.0f, true);
            }

            void DespawnCreaturesInArea(uint32 p_Entry, WorldObject* p_Object)
            {
                std::list<Creature*> l_Creatures;
                GetCreatureListWithEntryInGrid(l_Creatures, p_Object, p_Entry, 300.0f);

                for (Creature* l_Iter : l_Creatures)
                    l_Iter->DespawnOrUnsummon();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                _EnterCombat();

                events.ScheduleEvent(eEvents::EventFeedingFrenzy, urand(50 * TimeConstants::IN_MILLISECONDS, 60 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eEvents::EventBreakout, 20 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eEvents::EventPrimalAssault, urand(35 * TimeConstants::IN_MILLISECONDS, 40 * TimeConstants::IN_MILLISECONDS));
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo) override
            {
                if (m_HPPact)
                {
                    if (me->HealthBelowPctDamaged(m_HPPact, p_Damage))
                    {
                        me->CastStop();
                        me->RemoveAllAuras();
                        me->RemoveAura(eSpells::SpellFeedingFrenzy);
                        me->RemoveAura(eSpells::SpellFeedingAura);
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
                    me->m_Events.AddEvent(new cosmeticaloshir_event(me, 0), me->m_Events.CalculateTime(2 * TimeConstants::IN_MILLISECONDS));
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING) || me->HasAura(eSpells::SpellFeedingAura))
                    return;

                switch (events.ExecuteEvent())
                {
                    case eEvents::EventFeedingFrenzyCancel:
                    {
                        me->RemoveAura(eSpells::SpellFeedingFrenzy);
                        break;
                    }
                    case eEvents::EventFeedingFrenzy:
                    {
                        me->RemoveAura(eSpells::SpellFeedingFrenzy);

                        m_HPPact = me->GetHealthPct() * 0.95;

                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            me->CastSpell(l_Target, eSpells::SpellFeedingFrenzy);

                        events.ScheduleEvent(eEvents::EventFeedingFrenzy, urand(50 * TimeConstants::IN_MILLISECONDS, 60 * TimeConstants::IN_MILLISECONDS));
                        events.ScheduleEvent(eEvents::EventFeedingFrenzyCancel, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventPrimalAssault:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        {
                            me->CastSpell(l_Target, eSpells::SpellBreakout);
                            me->CastSpell(l_Target, eSpells::SpellPrimalAssault);
                        }

                        events.ScheduleEvent(eEvents::EventPrimalAssault, urand(35 * TimeConstants::IN_MILLISECONDS, 40 * TimeConstants::IN_MILLISECONDS));
                        break;
                    }
                    case eEvents::EventBreakout:
                    {
                        me->RemoveAura(eSpells::SpellFeedingAura);

                        switch (urand(0, 1))
                        {
                            case 0: ///< Wolfs
                            {
                                if (m_WolfDests.empty())
                                    return;

                                auto l_Iter = m_WolfDests.begin();
                                std::advance(l_Iter, urand(0, m_WolfDests.size() - 1));

                                Creature* l_Wolf = Unit::GetCreature(*me, *l_Iter);
                                if (l_Wolf)
                                {
                                    me->GetMotionMaster()->MoveJump(*l_Wolf, 25.0f, 10.0f, 10.0f);

                                    if (l_Wolf->GetAI())
                                    {
                                        l_Wolf->GetAI()->DoAction(eAction::ActionRelease);
                                        m_WolfDests.erase(l_Iter);
                                    }
                                }
                                break;
                            }
                            case 1: ///< Rylaks
                            {
                                if (m_WolfDests.empty())
                                    return;

                                auto l_Iter = m_RylakDests.begin();
                                std::advance(l_Iter, urand(0, m_RylakDests.size() - 1));

                                Creature* l_Rylak = Unit::GetCreature(*me, *l_Iter);
                                if (l_Rylak)
                                {
                                    me->GetMotionMaster()->MoveJump(*l_Rylak, 25.0f, 10.0f, 10.0f);

                                    if (l_Rylak->GetAI())
                                    {
                                        l_Rylak->GetAI()->DoAction(eAction::ActionRelease);
                                        m_RylakDests.erase(l_Iter);
                                    }
                                }
                                break;
                            }
                        }
                        events.ScheduleEvent(eEvents::EventBreakout, 20 * TimeConstants::IN_MILLISECONDS);
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
class iron_docks_mob_wolf_dest : public CreatureScript
{
    public:
        iron_docks_mob_wolf_dest() : CreatureScript("iron_docks_mob_wolf_dest") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = me->GetInstanceScript();
            }

            std::list<uint64> m_Wolfs;
            bool m_Released;
            InstanceScript* m_Instance;

            void Reset() override
            {
                m_Released = false;
                summons.DespawnAll();
                m_Wolfs.clear();

                for (uint8 l_I = 0; l_I <= 5; l_I++)
                {
                    if (Creature* l_Wolf = me->SummonCreature(eCreatures::CreatureWolf, *me, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        l_Wolf->setFaction(35);
                        l_Wolf->SetReactState(ReactStates::REACT_PASSIVE);
                        m_Wolfs.push_back(l_Wolf->GetGUID());
                    }
                }
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eAction::ActionRelease:
                    {
                        if (Creature* l_Oshir = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataOshir)))
                        {
                            if (!m_Wolfs.empty())
                            {
                                if (!m_Released)
                                {
                                    m_Released = true;
                                    events.ScheduleEvent(eEvents::EventRelease, 5 * TimeConstants::IN_MILLISECONDS);
                                }
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
                            if (l_Oshir->isInCombat() && l_Oshir->getVictim())
                            {
                                for (uint64 l_Guid : m_Wolfs)
                                {
                                    if (Creature* l_Wolf = Creature::GetCreature(*me, l_Guid))
                                    {
                                        if (!l_Wolf->IsInWorld() || !l_Wolf->isAlive())
                                            continue;

                                        me->GetMotionMaster()->Clear();
                                        l_Wolf->DespawnOrUnsummon(2 * TimeConstants::IN_MILLISECONDS);

                                        Creature* l_OtherWolf = me->SummonCreature(l_Wolf->GetEntry(), *l_Wolf, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                                        if (l_OtherWolf)
                                        {
                                            l_OtherWolf->SetSpeed(UnitMoveType::MOVE_RUN, 1.9f, true);
                                            l_OtherWolf->setFaction(16);
                                            l_OtherWolf->SetReactState(ReactStates::REACT_AGGRESSIVE);

                                            if (Player* l_Player = me->FindNearestPlayer(100.0f, true))
                                                l_OtherWolf->GetMotionMaster()->MovePoint(0, *l_Player);
                                        }
                                    }
                                }
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
class iron_docks_mob_rylak_dest : public CreatureScript
{
    public:
        iron_docks_mob_rylak_dest() : CreatureScript("iron_docks_mob_rylak_dest") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = me->GetInstanceScript();
            }

            std::list<uint64> m_Rylaks;
            bool m_Released;
            InstanceScript* m_Instance;

            void Reset() override
            {
                m_Released = false;
                summons.DespawnAll();
                m_Rylaks.clear();

                if (Creature* l_Rylak = me->SummonCreature(eCreatures::CreatureRylak, *me, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                {
                    l_Rylak->setFaction(35);
                    l_Rylak->SetReactState(ReactStates::REACT_PASSIVE);
                    m_Rylaks.push_back(l_Rylak->GetGUID());
                }
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eAction::ActionRelease:
                    {
                        if (Creature* l_Oshir = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataOshir)))
                        {
                            if (!m_Rylaks.empty())
                            {
                                if (!m_Released)
                                {
                                    m_Released = true;
                                    events.ScheduleEvent(eEvents::EventRelease, 5 * TimeConstants::IN_MILLISECONDS);
                                }
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
                            if (l_Oshir->isInCombat() && l_Oshir->getVictim())
                            {
                                for (uint64 l_Guid : m_Rylaks)
                                {
                                    if (Creature* l_Rylak = Creature::GetCreature(*me, l_Guid))
                                    {
                                        if (!l_Rylak->IsInWorld() || !l_Rylak->isAlive())
                                            continue;

                                        me->GetMotionMaster()->Clear();
                                        l_Rylak->DespawnOrUnsummon(2 * TimeConstants::IN_MILLISECONDS);

                                        Creature* l_OtherRylak = me->SummonCreature(l_Rylak->GetEntry(), *l_Rylak, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                                        if (l_OtherRylak)
                                        {
                                            l_OtherRylak->setFaction(16);
                                            l_OtherRylak->SetReactState(ReactStates::REACT_AGGRESSIVE);

                                            if (Player* l_Player = me->FindNearestPlayer(100.0f, true))
                                                l_OtherRylak->GetMotionMaster()->MovePoint(0, *l_Player);
                                        }
                                    }
                                }
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

/// Rylak Skyterror - 89011
class iron_docks_mob_rylak : public CreatureScript
{
    public:
        iron_docks_mob_rylak() : CreatureScript("iron_docks_mob_rylak") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = me->GetInstanceScript();
            }

            uint32 m_AcidTimer;
            InstanceScript* m_Instance;

            void Reset() override
            {
                m_AcidTimer = urand(11 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                if (m_AcidTimer <= p_Diff)
                {
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                    {
                        me->SummonCreature(eCreatures::TriggerAcidPool, *l_Target, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 15 * TimeConstants::IN_MILLISECONDS);
                        me->CastSpell(l_Target, eSpells::SpellAcidicCast);
                        m_AcidTimer = urand(11 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS);
                    }
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

class iron_docks_mob_wolf : public CreatureScript
{
    public:
        iron_docks_mob_wolf() : CreatureScript("iron_docks_mob_wolf") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = me->GetInstanceScript();
            }

            uint32 m_Timer;
            InstanceScript* m_Instance;

            void Reset() override
            {
                m_Timer = 1500;
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                if (m_Timer <= p_Diff)
                {
                    me->CastSpell(me, eSpells::SpellStrengthOfThePack);

                    std::list<Creature*> l_CreatureList;
                    me->GetCreatureListWithEntryInGrid(l_CreatureList, me->GetEntry(), 200.0f);

                    for (Creature* l_Iter : l_CreatureList)
                    {
                        if (l_Iter->IsInWorld() && l_Iter->isAlive() && l_Iter->getFaction() == 16)
                        {
                            if (AuraPtr l_Aura = me->GetAura(eSpells::SpellStrengthOfThePack))
                                l_Aura->SetStackAmount(l_CreatureList.size());
                        }
                    }

                    m_Timer = 1500;
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

class iron_docks_oshir_trigger_acid_spit : public CreatureScript
{
    public:
        iron_docks_oshir_trigger_acid_spit() : CreatureScript("iron_docks_oshir_trigger_acid_spit") { }

        struct mob_iron_docksAI : public Scripted_NoMovementAI
        {
            mob_iron_docksAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
            {
                m_Instance = me->GetInstanceScript();
            }

            uint32 m_Timer;
            InstanceScript* m_Instance;

            void Reset() override
            {
                m_Timer = 1 * TimeConstants::IN_MILLISECONDS;
                me->setFaction(16);
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

                    if (l_PlayerList.empty())
                        return;

                    for (Player* l_Iter : l_PlayerList)
                    {
                        if (!l_Iter->HasAura(eSpells::SpellAcidPool))
                        {
                            l_Iter->CastSpell(l_Iter, eSpells::SpellAcidPool);

                            if (l_Iter->HasAura(eSpells::SpellAcidPool))
                            {
                                if (AuraPtr l_Aura = l_Iter->GetAura(eSpells::SpellAcidPool))
                                    l_Aura->SetDuration(3);
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

    /// Spells
    new iron_docks_oshir_trigger_acid_spit();

    /// Adds
    new iron_docks_mob_rylak();
    new iron_docks_mob_wolf();

    /// Triggers
    new iron_docks_mob_rylak_dest();
    new iron_docks_mob_wolf_dest();
    new iron_docks_trigger_stand_fourth_event();
}