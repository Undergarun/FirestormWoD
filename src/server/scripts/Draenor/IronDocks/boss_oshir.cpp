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
    EventRelease = 1
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
    CreatureWolfDest     = 89022,
    CreatureRylakDest    = 89021,
    CreatureAcidPool     = 214746
};

enum eMovementInformed
{
    MovementInformOshirPrimalAssaultDestArrival = 1
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
                m_First = false;
                m_Instance = me->GetInstanceScript();
            }

            enum eOshirEvents
            {
                EventBreakout = 1,
                EventFeedingFrenzy,
                EventPrimalAssault,
                EventFeedingFrenzyCancel,
                EventIntro01,
                EventIntro02,
                EventIntro03
            };

            enum eOshirSpells
            {
                SpellFeedingAura         = 162424,
                SpellBreakout            = 178124,
                SpellFeedingFrenzy       = 162415,
                SpellPrimalAssault       = 161256,
                SpellPrimalAssaultVisual = 164733,
                SpellCosmeticAttack      = 42880
            };

            bool m_Intro;
            bool m_First;
            bool m_PrimalAssault;
            uint32 m_HpPact;
            uint32 m_PrimalAssaultDiff;
            std::list<uint64> m_WolfDests;
            std::list<uint64> m_RylakDests;
            std::list<uint64> m_ThunderilingWandlers;
            InstanceScript* m_Instance;

            void Reset() override
            {
                _Reset();
                events.Reset();
                RespawnDests();
                m_WolfDests.clear();
                m_RylakDests.clear();         
                m_PrimalAssault = false;
                m_HpPact = me->GetHealthPct() * 0.95;
                m_PrimalAssaultDiff = 1 * TimeConstants::IN_MILLISECONDS;
                DespawnCreaturesInArea(eCreatures::CreatureWolf, me);
                DespawnCreaturesInArea(eCreatures::CreatureRylak, me);
                me->SetSpeed(UnitMoveType::MOVE_FLIGHT, 10.0f, true);   
                if (!m_First)
                {
                    m_First = true;
                    me->setFaction(FriendlyFaction);
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_UNK_15 | eUnitFlags::UNIT_FLAG_UNK_6 | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
                    ActivateEncounterGate();
                }
            }

            void ActivateEncounterGate()
            {
                if (m_Instance != nullptr)
                {
                    if (GameObject* l_OshirEncounterGate = m_Instance->instance->GetGameObject(m_Instance->GetData64(eIronDocksDatas::DataEncounterGateOshir)))
                    {
                        l_OshirEncounterGate->SetLootState(LootState::GO_READY);
                        l_OshirEncounterGate->UseDoorOrButton(10 * TimeConstants::IN_MILLISECONDS, false, me);
                    }
                }
            }

            void RespawnDests()
            {
                std::list<Creature*> l_WolfsTempList;
                std::list<Creature*> l_RylaksTempList;
                me->GetCreatureListWithEntryInGrid(l_WolfsTempList, eCreatures::CreatureWolfDest, 300.0f);
                me->GetCreatureListWithEntryInGrid(l_RylaksTempList, eCreatures::CreatureRylakDest, 300.0f);
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

            void JustReachedHome() override
            {
                _JustReachedHome();
                summons.DespawnAll();
                ActivateEncounterGate();
                DespawnCreaturesInArea(eCreatures::CreatureWolf, me);
                DespawnCreaturesInArea(eCreatures::CreatureRylak, me);  
                /// Reset cages
                uint32 l_CagesEntries[7] = { eIronDocksGameObject::GameObjectIronHordeCageDoor01, eIronDocksGameObject::GameObjectIronHordeCageDoor02,
                    eIronDocksGameObject::GameObjectIronHordeCageDoor03, eIronDocksGameObject::GameObjectIronHordeCageDoor04,
                    eIronDocksGameObject::GameObjectIronHordeCageDoor05, eIronDocksGameObject::GameObjectIronHordeCageDoor06, eIronDocksGameObject::GameObjectIronHordeCageDoor07 };
                std::list<GameObject*> l_ListRylakWolfsCages;
                for (uint32 l_I = 0; l_I <= 6; l_I++)
                {
                    me->GetGameObjectListWithEntryInGrid(l_ListRylakWolfsCages, l_CagesEntries[l_I], 300.0f);
                }
                if (!l_ListRylakWolfsCages.empty())
                {
                    for (auto itr : l_ListRylakWolfsCages)
                    {
                        // Activate
                        itr->SetLootState(LootState::GO_READY);
                        itr->UseDoorOrButton(10 * TimeConstants::IN_MILLISECONDS, false, me);
                    }
                }
                me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                me->RemoveAura(eOshirSpells::SpellPrimalAssaultVisual);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);      
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                _EnterCombat();
                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
                    DoZoneInCombat();
                }     

                RespawnDests();
                me->SetHomePosition(*me);
                ActivateEncounterGate();
                events.ScheduleEvent(eOshirEvents::EventFeedingFrenzy, urand(18 * TimeConstants::IN_MILLISECONDS, 25 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eOshirEvents::EventBreakout, 10 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eOshirEvents::EventPrimalAssault, 36 * TimeConstants::IN_MILLISECONDS);
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

            void MovementInform(uint32 /*p_Type*/, uint32 p_Id) override
            {
                if (p_Id == eMovementInformed::MovementInformOshirPrimalAssaultDestArrival)
                {
                    m_PrimalAssault = false;
                    me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                    me->RemoveAura(eOshirSpells::SpellPrimalAssaultVisual);
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);           
                }
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                _JustDied();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
                }

                ActivateEncounterGate();
                uint32 l_Entries[4] = { eCreatures::CreatureWolfDest, eCreatures::CreatureWolf, eCreatures::CreatureRylakDest, eCreatures::CreatureRylak};
                for (uint32 l_I = 0; l_I <= 3; l_I++)
                {
                    DespawnCreaturesInArea(l_Entries[l_I], me);
                }
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(p_Who, 50.0f) && !m_Intro)
                {
                    for (uint8 l_I = 0; l_I < 2; l_I++)
                    {
                        if (Creature* l_Guard = me->SummonCreature(eCreatures::CreatureGuard, g_NpcSpawnPos, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                        {
                            m_ThunderilingWandlers.push_back(l_Guard->GetGUID());
                            l_Guard->GetMotionMaster()->MovePoint(0, g_NpcMovePos[l_I]);
                            l_Guard->setFaction(FriendlyFaction);
                        }
                    }

                    m_Intro = true;
                    events.ScheduleEvent(eOshirEvents::EventIntro01, 2 * TimeConstants::IN_MILLISECONDS);
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                events.Update(p_Diff);

                if (!m_Intro)
                {
                    switch (events.ExecuteEvent())
                    {
                    case eOshirEvents::EventIntro01:
                        if (Creature* l_Guard = Creature::GetCreature(*me, m_ThunderilingWandlers.back()))
                        {
                            if (GameObject* l_GameObject = l_Guard->FindNearestGameObject(eIronDocksGameObject::GameObjectIronHordeCageDoor07, 10.0f))
                            {
                                l_GameObject->SetLootState(LootState::GO_READY);
                                l_GameObject->UseDoorOrButton(10 * TimeConstants::IN_MILLISECONDS, false, l_Guard);
                                me->GetMotionMaster()->MoveChase(l_Guard);
                                me->CastSpell(l_Guard, eOshirSpells::SpellCosmeticAttack);
                                me->Kill(l_Guard);
                                l_Guard->DespawnOrUnsummon(5 * TimeConstants::IN_MILLISECONDS);
                                m_ThunderilingWandlers.remove(m_ThunderilingWandlers.back());
                            }
                        }
                        events.ScheduleEvent(eOshirEvents::EventIntro02, 4 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eOshirEvents::EventIntro02:
                        if (Creature* l_Guard = Creature::GetCreature(*me, m_ThunderilingWandlers.back()))
                        {
                            me->GetMotionMaster()->MoveChase(l_Guard);
                            me->CastSpell(l_Guard, eOshirSpells::SpellCosmeticAttack);
                            me->Kill(l_Guard);
                            l_Guard->DespawnOrUnsummon(5 * TimeConstants::IN_MILLISECONDS);
                            m_ThunderilingWandlers.remove(m_ThunderilingWandlers.back());
                            events.ScheduleEvent(eOshirEvents::EventIntro03, 5 * TimeConstants::IN_MILLISECONDS);
                        }
                        break;
                    case eOshirEvents::EventIntro03:
                        if (Creature* l_Guard = Creature::GetCreature(*me, m_ThunderilingWandlers.back()))
                        {
                            me->CastSpell(me, eOshirSpells::SpellPrimalAssaultVisual);
                            l_Guard->GetMotionMaster()->MovePoint(eIronDocksMovementInformed::MovementInformWanlderingThundlerOshirThirdKill, 6923.703f, -1131.837f, 4.603416f);
                            m_ThunderilingWandlers.remove(m_ThunderilingWandlers.back());
                        }
                        break;
                    }
                }


                if (!UpdateVictim())
                    return;

                events.Update(p_Diff);

                ///< Primal Assault
                if (m_PrimalAssault)
                {
                    if (m_PrimalAssaultDiff <= p_Diff)
                    {
                        std::list<Player*> l_ListPlayers;
                        me->GetPlayerListInGrid(l_ListPlayers, 2.0f);
                        if (!l_ListPlayers.empty())
                        {

                            for (auto l_Itr : l_ListPlayers)
                            {
                                if (!l_Itr->isInFront(me, M_PI))
                                    continue;

                                l_Itr->AddAura(eOshirSpells::SpellPrimalAssault, l_Itr);

                                if (AuraPtr l_Aura = l_Itr->GetAura(eOshirSpells::SpellPrimalAssault))
                                {
                                    l_Aura->SetDuration(1 * TimeConstants::IN_MILLISECONDS);
                                }
                            }
                        }
                        else
                            m_PrimalAssaultDiff -= p_Diff;
                    }
                }

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING) || me->HasAura(eOshirSpells::SpellFeedingAura) || m_PrimalAssault)
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
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_FARTHEST, 0, 50.0f, true))
                        {
                            m_PrimalAssault = true;
                            m_PrimalAssaultDiff = 1 * TimeConstants::IN_MILLISECONDS;
                            me->CastSpell(l_Target, eOshirSpells::SpellPrimalAssault);
                            me->CastSpell(me, eOshirSpells::SpellPrimalAssaultVisual, true);
                            me->SetReactState(ReactStates::REACT_PASSIVE);
                            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
                            me->GetMotionMaster()->MoveCharge(l_Target, 10.0f, eMovementInformed::MovementInformOshirPrimalAssaultDestArrival);
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
            std::list<uint64> l_ListWolvesGuid;
            bool m_Released;

            void Reset() override
            {  
                events.Reset();
                m_Released = false;
              
                for (uint8 l_I = 0; l_I <= 4; l_I++)
                {
                    Position l_Pos;
                    me->GetRandomNearPosition(l_Pos, 1.5f);
                    if (Creature* l_Wolf = me->SummonCreature(eCreatures::CreatureWolf, l_Pos, TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
                        l_ListWolvesGuid.push_back(l_Wolf->GetGUID());
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
                            if (l_ListWolvesGuid.empty())
                                return;

                            for (auto l_Itr : l_ListWolvesGuid)
                            {
                                if (Creature* l_Creature = Creature::GetCreature(*me, l_Itr))
                                {
                                    l_Creature->setFaction(HostileFaction);
                                    l_Creature->SetReactState(ReactStates::REACT_AGGRESSIVE);
                                    l_Creature->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
                                    if (Player* l_Player = me->FindNearestPlayer(500.0f, true))
                                    {
                                        l_Creature->Attack(l_Player, true);
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
            std::list<uint64> l_ListRylaksGuid;
            bool m_Released;

            void Reset() override
            {
                events.Reset();
                m_Released = false;

                if (Creature* l_Rylak = me->SummonCreature(eCreatures::CreatureRylak, *me, TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
                    l_ListRylaksGuid.push_back(l_Rylak->GetGUID());
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
                        if (l_ListRylaksGuid.empty())
                            return;

                        for (auto l_Itr : l_ListRylaksGuid)
                        {
                            if (Creature* l_Creature = Creature::GetCreature(*me, l_Itr))
                            {
                                l_Creature->setFaction(HostileFaction);
                                l_Creature->SetReactState(ReactStates::REACT_AGGRESSIVE);
                                l_Creature->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
                                if (Player* l_Player = me->FindNearestPlayer(500.0f, true))
                                    l_Creature->Attack(l_Player, true);
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

        struct iron_docks_oshir_mob_rylakAI : public ScriptedAI
        {
            iron_docks_oshir_mob_rylakAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = me->GetInstanceScript();
   
            }

            enum eAcidSpitSpells
            {
                /// Rylak
                SpellAcidSpitDummy             = 178154,
                SpellAcidSpitTriggerMissile    = 178155,
                SpellAcidSpitDamageAreaTrigger = 178157,
                SpellAcidSplash                = 178156
            };


            uint32 m_AcidTimer;
            InstanceScript* m_Instance;

            void Reset() override
            {
                me->setFaction(FriendlyFaction);
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
                m_AcidTimer = urand(11 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                if (Unit* l_Victim = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO, 0, 100.0f, true))
                {
                    if (!me->IsWithinMeleeRange(l_Victim))
                        me->GetMotionMaster()->MoveChase(l_Victim);
                }

                if (m_AcidTimer <= p_Diff)
                {
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                    {
                        me->CastSpell(l_Target, eAcidSpitSpells::SpellAcidSpitDummy);
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
            return new iron_docks_oshir_mob_rylakAI(p_Creature);
        }
};

/// Wolf (Loup Vorace) - 89012
class iron_docks_oshir_mob_wolf : public CreatureScript
{
    public:

        iron_docks_oshir_mob_wolf() : CreatureScript("iron_docks_oshir_mob_wolf") { }

        struct iron_docks_oshir_mob_wolfAI : public ScriptedAI
        {
            iron_docks_oshir_mob_wolfAI(Creature* p_Creature) : ScriptedAI(p_Creature)
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
                me->setFaction(FriendlyFaction);
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
                m_Timer = 1 * TimeConstants::IN_MILLISECONDS;
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                me->DespawnOrUnsummon();
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                           if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                if (Unit* l_Victim = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO, 0, 100.0f, true))
                {
                    if (!me->IsWithinMeleeRange(l_Victim))
                        me->GetMotionMaster()->MoveChase(l_Victim);
                }
                
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
            return new iron_docks_oshir_mob_wolfAI(p_Creature);
        }
};

/// Acid Spit - 324256 // Custom
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

/// Acid Spit Dummy - 178154 
class iron_docks_oshir_spell_acid_spit : public SpellScriptLoader
{
public:

    iron_docks_oshir_spell_acid_spit() : SpellScriptLoader("iron_docks_oshir_spell_acid_spit") { }

    class iron_docks_oshir_spell_acid_spit_SpellScript : public SpellScript
    {
        PrepareSpellScript(iron_docks_oshir_spell_acid_spit_SpellScript);

        enum eAcidSpitSpells
        {
            SpellAcidSpitTriggerMissile = 178155
        };

        void HandleDummy(SpellEffIndex p_EffIndex)
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (l_Caster->IsAIEnabled)
                {
                    if (Unit* l_Target = l_Caster->GetAI()->SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                        l_Caster->CastSpell(l_Target, eAcidSpitSpells::SpellAcidSpitTriggerMissile, true);
                }
            }
        }

        void Register() override
        {
            OnEffectLaunch += SpellEffectFn(iron_docks_oshir_spell_acid_spit_SpellScript::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new iron_docks_oshir_spell_acid_spit_SpellScript();
    }
};

/// Acid Spit Trigger Missile - 178155 
class iron_docks_oshir_spell_acid_spit_trigger_missile : public SpellScriptLoader
{
public:
    iron_docks_oshir_spell_acid_spit_trigger_missile() : SpellScriptLoader("iron_docks_oshir_spell_acid_spit_trigger_missile") { }

    enum eAcidSpitCreatures
    {
        CreatureAcidSpitTrigger = 324256
    };

    class iron_docks_oshir_spell_acid_spit_trigger_missile_SpellScript : public SpellScript
    {
        PrepareSpellScript(iron_docks_oshir_spell_acid_spit_trigger_missile_SpellScript);

        void OnSpellHit(SpellEffIndex)
        {
            Unit* l_Caster = GetCaster();
            WorldLocation const* l_Dest = GetExplTargetDest();

            if (!l_Caster || !l_Dest)
                return;

            l_Caster->SummonCreature(eAcidSpitCreatures::CreatureAcidSpitTrigger, *l_Dest, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 30 * TimeConstants::IN_MILLISECONDS);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(iron_docks_oshir_spell_acid_spit_trigger_missile_SpellScript::OnSpellHit, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_TRIGGER_MISSILE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new iron_docks_oshir_spell_acid_spit_trigger_missile_SpellScript();
    }
};

void AddSC_boss_oshir()
{
    /// Boss
    new boss_oshir(); /// 79852
    /// Mobs
    new iron_docks_oshir_mob_rylak(); /// 89011
    new iron_docks_oshir_mob_wolf(); /// 89012
    new iron_docks_oshir_mob_acid_spit(); /// 324256
    /// Triggers
    new iron_docks_oshir_mob_rylak_dest(); /// 89021
    new iron_docks_oshir_mob_wolf_dest(); /// 89022
    new iron_docks_oshir_mob_event();
    /// Spells
    new iron_docks_oshir_spell_acid_spit(); /// 178154
    new iron_docks_oshir_spell_acid_spit_trigger_missile(); /// 178155
}