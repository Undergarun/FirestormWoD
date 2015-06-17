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
    /// Nok'gar
    SayAggro        = 0,    ///< Start the funeral pyres! (46057)
    SaySlay         = 1,    ///< Hahahaha my blade's thirst for war! (46061) / Revolt.. in the slaughter! (46060)
    SayDeath        = 2,    ///< You.. will burn.. with me. (46058)
    SayIntro        = 3,    ///< Warsong arrows sing from the skies, they'll be the last thing you hear as I crush your skull. (46059)
    SaySpell03      = 6,    ///< Terror overwhelms you.. Death is near! (46064)
    SaySpell04      = 7,    ///< A death worthy of a warrior. (46065)

    /// After Death
    TalkZoggosh01   = 8,    ///< Sir, they've breached the gates! Nok'gar is.. dead!! We should pick anchor and prepare to sail to Talador. Right now! (44047)
    TalkKoramar01   = 23,   ///< Calm yourself.. Zoggosh. We'll do no such thing, if these weaklings are so eager to die then we should obliged! (43899)
    TalkKoramar0222 = 24    ///< Zoggosh.. do not question my authority. This isn't just any Groon, this is Skuloc son of Grool.. the blood of a champion course through his veins.. I'm not concerned with these whelps. (43900)  
};

enum eSpells
{
    SpellBarbedArrow                = 164370,
    SpellRecklessProvocation        = 164426,
    SpellIntimidated                = 164504,
    /// Wolf - Dreadfang
    SpellBloodlettingHowl           = 164835,
    SpellSavageMauling              = 154039,
    SpellShreddingSwipesDoT         = 164734,
    SpellShreddingSwipesJump        = 164735,
    SpellShreddingSwipesDummy       = 164730,
    SpellShreddingSwipesAuraRemove  = 164733,
    /// Adds
    SpellBarbedArrowAT              = 164278,
    SpellBurningArrowAT             = 164234,
    /// MISC
    SpellWarsongFlag                = 168531,
    SpellFeignDeath                 = 103750,
    SpellCosmeticFeignDeath         = 166925
};

enum eEvents
{
    EventFireArrows             = 900,
    EventBarbedArrows           = 902,
    EventRecklessProvocation    = 903,
    EventDismount               = 904,
    /// Wolf
    EventBloodlettingHowl       = 905,
    EventSavageMauling          = 906,
    EventShreddingSwipes        = 907,
    EventShreddingSwipes2       = 908,
    EventFireArrowsCheck        = 910
};

enum eActions
{
    ActionDismountHeroic    = 1000,
    ActionFireArrows        = 999
};

enum eVehicle
{
    CreatureWolf = 81297
};

enum eTrigger
{
    ShreddingSwipesTrigger = 81832
};

Position const g_ArchersPos[5] =
{
    { 6882.93f, -694.61f, 55.554f, 3.14270f },
    { 6883.21f, -705.07f, 55.922f, 3.13948f },
    { 6883.21f, -688.00f, 56.686f, 3.16305f },
    { 6884.02f, -676.18f, 56.483f, 3.34997f },
    { 6884.07f, -662.27f, 56.541f, 3.09864f }
};

class Nokgar_Death_Event : public BasicEvent
{
    public:
        explicit Nokgar_Death_Event(Unit* p_Unit, int p_Value) : m_Obj(p_Unit), m_Modifier(p_Value) { }

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
                            l_Skulloc->GetMap()->SetObjectVisibility(1000.0f);
                            l_Koramar->GetMap()->SetObjectVisibility(1000.0f);
                            l_Zoggosh->GetMap()->SetObjectVisibility(1000.0f);

                            if (l_Zoggosh->IsAIEnabled && l_Koramar->IsAIEnabled)
                            {
                                switch (m_Modifier)
                                {
                                    case 0:
                                        l_Zoggosh->AI()->Talk(eYells::TalkZoggosh01);
                                        l_Zoggosh->m_Events.AddEvent(new Nokgar_Death_Event(l_Zoggosh, 1), l_Zoggosh->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    case 1:
                                        l_Koramar->AI()->Talk(eYells::TalkKoramar01);
                                        l_Koramar->m_Events.AddEvent(new Nokgar_Death_Event(l_Koramar, 2), l_Koramar->m_Events.CalculateTime(12 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    case 2:
                                        l_Koramar->AI()->Talk(eYells::TalkKoramar0222);
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

/// Dreadfang <Fleshrender Nok'gar's Mount> - 81297
class boss_mount_wolf : public CreatureScript
{
    public:
        boss_mount_wolf() : CreatureScript("boss_mount_wolf") { }

        struct boss_mount_wolfAI : public BossAI
        {
            boss_mount_wolfAI(Creature* p_Creature) : BossAI(p_Creature, eIronDocksDatas::DataMountWolf), m_Vehicle(p_Creature->GetVehicleKit())
            {
                me->Respawn(true);
                Reset();
            }

            Vehicle* m_Vehicle;
            InstanceScript* m_Instance = me->GetInstanceScript();
            uint32 m_ShreddingStrikeTimer;

            void Reset() override
            {
                _Reset();
                events.Reset();

                ASSERT(m_Vehicle);

                me->RemoveFlag(EObjectFields::OBJECT_FIELD_DYNAMIC_FLAGS, UnitDynFlags::UNIT_DYNFLAG_DEAD);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN | eUnitFlags2::UNIT_FLAG2_FEIGN_DEATH);
                me->RemoveUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);

                DespawnCreaturesInArea(eIronDocksCreatures::NpcGromkarFlameslinger, me);
                me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                m_ShreddingStrikeTimer = 300;
            }

            void DespawnCreaturesInArea(uint32 p_Entry, WorldObject* p_Object)
            {
                std::list<Creature*> l_CreatureList;
                GetCreatureListWithEntryInGrid(l_CreatureList, p_Object, p_Entry, 300.0f);

                for (Creature* l_Iter : l_CreatureList)
                    l_Iter->DespawnOrUnsummon();
            }

            void EnterCombat(Unit* /*p_Who*/) override
            {
                _EnterCombat();
                Talk(eYells::SayAggro);

                events.ScheduleEvent(eEvents::EventSavageMauling, urand(10 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eEvents::EventBloodlettingHowl, 20 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eEvents::EventShreddingSwipes, urand(8 * TimeConstants::IN_MILLISECONDS, 30 * TimeConstants::IN_MILLISECONDS));
            }

            void KilledUnit(Unit* p_Killed) override
            {
                if (p_Killed->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eYells::SaySlay);
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo) override
            {
                if (me->GetHealthPct() <= 12)
                {
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

                    events.CancelEvent(eEvents::EventSavageMauling);
                    events.CancelEvent(eEvents::EventBloodlettingHowl);
                    events.CancelEvent(eEvents::EventShreddingSwipes);

                    me->RemoveAllAuras();
                    me->SetFullHealth();
                    me->CastSpell(me, eSpells::SpellFeignDeath);
                    me->CastSpell(me, eSpells::SpellCosmeticFeignDeath);


                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_FEIGN_DEATH);
                    me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                    me->SetFlag(EObjectFields::OBJECT_FIELD_DYNAMIC_FLAGS, UnitDynFlags::UNIT_DYNFLAG_DEAD);
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);

                    me->GetVehicleKit()->RemoveAllPassengers();

                    if (Creature* l_Nokgar = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataNokgar)))
                    {
                        if (l_Nokgar->IsAIEnabled)
                            l_Nokgar->AI()->Talk(eYells::SaySpell04);
                    }
                }

                if (me->GetHealthPct() <= 50) ///< Heroic
                {
                    if (Creature* l_Nokgar = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataNokgar)))
                    {
                        if (l_Nokgar->IsAIEnabled)
                            l_Nokgar->AI()->DoAction(eActions::ActionDismountHeroic);
                    }
                }
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                _JustDied();
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who && p_Who->IsInWorld() && p_Who->GetEntry() == eTrigger::ShreddingSwipesTrigger && me->IsWithinDistInMap(p_Who, 1.0f))
                {
                    p_Who->ToCreature()->DespawnOrUnsummon();
                    me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                    me->RemoveAura(eSpells::SpellShreddingSwipesAuraRemove);
                }
            }

            void JustReachedHome() override
            {
                if (Creature* l_Nokgar = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataNokgar)))
                    l_Nokgar->DespawnOrUnsummon();

                me->DespawnOrUnsummon(1 * TimeConstants::IN_MILLISECONDS);
                me->SummonCreature(me->GetEntry(), *me, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(p_Diff);

                /// I think blizzard actually hacked it themselves aswell.
                if (me->HasAura(eSpells::SpellShreddingSwipesAuraRemove))
                {
                    if (m_ShreddingStrikeTimer <= p_Diff)
                    {
                        std::list<Player*> l_PlayerList;

                        JadeCore::AnyPlayerInObjectRangeCheck l_Check(me, 3.0f);
                        JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> l_Searcher(me, l_PlayerList, l_Check);
                        me->VisitNearbyObject(3.0f, l_Searcher);

                        for (Player* l_Iter : l_PlayerList)
                        {
                            if (me->isInFront(l_Iter, M_PI * 0.5f))
                                l_Iter->AddAura(eSpells::SpellShreddingSwipesDummy, l_Iter);
                        }

                        m_ShreddingStrikeTimer = 350;
                    }
                    else
                        m_ShreddingStrikeTimer -= p_Diff;
                }

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (events.ExecuteEvent())
                {
                    case eEvents::EventSavageMauling:
                    {
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        {
                            me->GetMotionMaster()->MoveJump(*l_Target, 8.0, 4.0f);
                            me->Attack(l_Target, true);
                            me->CastSpell(l_Target, eSpells::SpellSavageMauling);

                            events.ScheduleEvent(eEvents::EventSavageMauling, urand(10 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
                        }

                        break;
                    }
                    case eEvents::EventBloodlettingHowl:
                    {
                        me->CastSpell(me, eSpells::SpellBloodlettingHowl);
                        events.ScheduleEvent(eEvents::EventBloodlettingHowl, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventShreddingSwipes:
                    {
                        me->AddAura(eSpells::SpellShreddingSwipesAuraRemove, me);
                        me->SetReactState(ReactStates::REACT_PASSIVE);
                        events.ScheduleEvent(eEvents::EventShreddingSwipes, urand(8 * TimeConstants::IN_MILLISECONDS, 30 * TimeConstants::IN_MILLISECONDS));
                        events.ScheduleEvent(eEvents::EventShreddingSwipes2, 500);
                        break;
                    }
                    case eEvents::EventShreddingSwipes2:
                    {
                        if (Creature* l_ShreddingTrigger = me->FindNearestCreature(eTrigger::ShreddingSwipesTrigger, 100.0F, true))
                        {
                            /// Dafuq?
                            Position l_Pos;
                            me->GetPosition(&l_Pos);
                            me->GetMotionMaster()->MovePoint(0, l_Pos);
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
            return new boss_mount_wolfAI(p_Creature);
        }
};

/// Fleshrender Nok'gar - 81305
class boss_nokgar : public CreatureScript
{
    public:
        boss_nokgar() : CreatureScript("boss_nokgar") { }

        struct boss_nokgarAI : public BossAI
        {
            boss_nokgarAI(Creature* p_Creature) : BossAI(p_Creature, eIronDocksDatas::DataNokgar)
            {
                m_Intro = false;
            }

            int8 m_Phase;
            bool m_Intro;
            bool m_DismountHeroic;
            InstanceScript* m_Instance = me->GetInstanceScript();

            void Reset() override
            {
                _Reset();
                events.Reset();
                summons.DespawnAll();
                SummonArchers();

                me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                m_Phase = 0;
                me->CastSpell(me, eSpells::SpellWarsongFlag);
                m_DismountHeroic = false;
            }

            void SummonArchers()
            {
                for (uint8 l_I = 0; l_I < 5; l_I++)
                    me->SummonCreature(eIronDocksCreatures::NpcGromkarFlameslinger, g_ArchersPos[l_I], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
            }

            void LaunchArchers()
            {
                std::list<Creature*> l_Flameslingers;
                me->GetCreatureListWithEntryInGrid(l_Flameslingers, eIronDocksCreatures::NpcGromkarFlameslinger, 100.0f);

                for (Creature* l_Iter : l_Flameslingers)
                {
                    if (l_Iter->IsAIEnabled)
                        l_Iter->GetAI()->DoAction(eActions::ActionFireArrows);
                }
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && !m_Intro && me->IsWithinDistInMap(p_Who, 10.0f))
                {
                    m_Intro = true;
                    Talk(eYells::SayIntro);
                }
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo) override
            {
                if (me->HealthBelowPct(50) && !m_DismountHeroic)
                {
                    m_DismountHeroic = true;
                    events.ScheduleEvent(eEvents::EventDismount, 1 * TimeConstants::IN_MILLISECONDS);
                }

                if (me->HasAura(eSpells::SpellRecklessProvocation))
                {
                    if (!p_SpellInfo)
                        me->AddAura(eSpells::SpellIntimidated, p_Attacker);
                }
            }

            void EnterCombat(Unit* p_Who) override
            {
                _EnterCombat();
                Talk(eYells::SayAggro);
                LaunchArchers();

                if (m_Instance != nullptr)
                {
                    if (Creature* l_Wolf = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataMountWolf)))
                    {
                        l_Wolf->Attack(p_Who, true);
                        me->Attack(p_Who, true);
                    }
                }

                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

                if (!IsHeroic())
                    events.ScheduleEvent(eEvents::EventDismount, 29 * TimeConstants::IN_MILLISECONDS);
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
                summons.DespawnAll();

                if (m_Instance != nullptr)
                {
                    if (Creature* l_Wolf = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataMountWolf)))
                        l_Wolf->DespawnOrUnsummon(2 * TimeConstants::IN_MILLISECONDS);
                }

                me->m_Events.AddEvent(new Nokgar_Death_Event(me, 0), me->m_Events.CalculateTime(2 * TimeConstants::IN_MILLISECONDS));
            }

            void JustReachedHome() override
            {
                if (m_Instance == nullptr)
                    return;

                if (Creature* l_Wolf = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataMountWolf)))
                {
                    summons.DespawnAll();
                    l_Wolf->DespawnOrUnsummon(1 * TimeConstants::IN_MILLISECONDS);
                    me->DespawnOrUnsummon(1 * TimeConstants::IN_MILLISECONDS);

                    me->SummonCreature(eVehicle::CreatureWolf, *me, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                }
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eActions::ActionDismountHeroic:
                    {
                        events.ScheduleEvent(eEvents::EventDismount, 1 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
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
                    case eEvents::EventDismount:
                    {
                        if (m_Instance != nullptr)
                        {
                            if (Creature* wolf = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataMountWolf)))
                                wolf->GetVehicleKit()->RemoveAllPassengers(true);
                        }

                        me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                        events.SetPhase(1);
                        events.ScheduleEvent(eEvents::EventRecklessProvocation, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventRecklessProvocation:
                    {
                        Talk(eYells::SaySpell03);
                        me->MonsterTextEmote("Fleshrender Nok'gar casts |cffff0000[Reckless Provocation]|cfffaeb00!", me->GetGUID());
                        me->CastSpell(me, eSpells::SpellRecklessProvocation);
                        events.ScheduleEvent(eEvents::EventRecklessProvocation, urand(15 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_nokgarAI(p_Creature);
        }
};

/// Grom'kar Footsoldier - 81279
class iron_docks_flameslinger : public CreatureScript
{
    public:
        iron_docks_flameslinger() : CreatureScript("iron_docks_flameslinger") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                me->SetDisableGravity(true);
                me->SetHover(true);
            }

            int32 m_DShot;
            int32 m_EShot;

            void Reset() override
            {
                m_DShot = 0;
                m_EShot = 0;
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                me->setFaction(16);
                me->RemoveAllAuras();
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eActions::ActionFireArrows:
                        events.ScheduleEvent(eEvents::EventFireArrows, 2 * TimeConstants::IN_MILLISECONDS);
                        events.ScheduleEvent(eEvents::EventBarbedArrows, 12 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                events.Update(p_Diff);

                switch (events.ExecuteEvent())
                {
                    case eEvents::EventFireArrows:
                    {
                        if (Player* l_Target = me->FindNearestPlayer(200.0f, true))
                            me->CastSpell(l_Target, eSpells::SpellBurningArrowAT, true);

                        events.ScheduleEvent(eEvents::EventFireArrows, 4 * TimeConstants::IN_MILLISECONDS);

                        m_DShot++;
                        if (m_DShot >= 4)
                        {
                            m_DShot = 0;
                            events.CancelEvent(eEvents::EventFireArrows);
                            events.ScheduleEvent(eEvents::EventFireArrowsCheck, 1 * TimeConstants::IN_MILLISECONDS);
                        }

                        break;
                    }
                    case eEvents::EventBarbedArrows:
                    {
                        if (Player* l_Target = me->FindNearestPlayer(200.0f, true))
                            me->CastSpell(l_Target, eSpells::SpellBarbedArrowAT, true);

                        events.ScheduleEvent(eSpells::SpellBarbedArrow, 6 * TimeConstants::IN_MILLISECONDS);
                        m_EShot++;

                        if (m_EShot >= 4)
                        {
                            m_EShot = 0;
                            events.CancelEvent(eEvents::EventBarbedArrows);
                            events.ScheduleEvent(eEvents::EventFireArrowsCheck, 1 * TimeConstants::IN_MILLISECONDS);
                        }

                        break;
                    }
                    case eEvents::EventFireArrowsCheck:
                    {
                        m_EShot = 0;
                        m_DShot = 0;
                        events.ScheduleEvent(eSpells::SpellBarbedArrow, 20 * TimeConstants::IN_MILLISECONDS);
                        events.ScheduleEvent(eEvents::EventFireArrows, 20 * TimeConstants::IN_MILLISECONDS);
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

/// Intimidated - 164504
class iron_docks_intimidated : public SpellScriptLoader
{
    public:
        iron_docks_intimidated() : SpellScriptLoader("iron_docks_intimidated") { }

        class iron_docks_spells : public SpellScript
        {
            PrepareSpellScript(iron_docks_spells);

            SpellCastResult CheckTarget()
            {
                if (GetCaster()->GetEntry() == eIronDocksCreatures::BossFleshrenderNokgar)
                    return SpellCastResult::SPELL_FAILED_DONT_REPORT;
                else
                    return SpellCastResult::SPELL_CAST_OK;
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(iron_docks_spells::CheckTarget);
            }
        };
};

void AddSC_boss_nokgar()
{
    new boss_nokgar();
    new iron_docks_flameslinger();
    new boss_mount_wolf();
    new iron_docks_intimidated();
}